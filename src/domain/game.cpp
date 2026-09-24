#include "game.hpp"

#include "domain/event.hpp"
#include "hand.hpp"

#include <stdexcept>

namespace domain {

void Game::deal_initial_cards() {
    if (state_ != GameState::Dealing) {
        throw std::runtime_error("Cannot deal cards at this time.");
    }

    for (int i = 0; i < 2; ++i) {
        deal_card_to_player();
    }

    deal_card_to_dealer();
    deal_card_to_dealer(false);
}

void Game::deal_dealer_cards() {
    if (state_ != GameState::DealerTurn) {
        throw std::runtime_error("Cannot deal dealer cards at this time.");
    }

    dealer_.reveal_hole_card();
    events_.emplace_back(HoleCardRevealedEvent{dealer_.hand().cards()[1].card});

    while (dealer_.hand().is_soft() && dealer_.hand().value() < 17) {
        deal_card_to_dealer();
    }

    for (size_t i = 0; i < player_.hand_count(); ++i) {
        evaluate_hand(i);
    }
}

void Game::hit() {
    if (state_ != GameState::PlayerTurn) {
        throw std::runtime_error("Cannot hit at this time.");
    }

    deal_card_to_player();

    if (player_.active_hand().hand.is_busted()) {
        player_.set_hand_outcome(HandOutcome::Busted);
        player_.advance_to_next_hand();
        events_.emplace_back(HandBustedEvent{});
    }
}

void Game::stand() {
    if (state_ != GameState::PlayerTurn) {
        throw std::runtime_error("Cannot stand at this time.");
    }

    player_.stand();
    player_.advance_to_next_hand();
}

void Game::double_down() {
    if (state_ != GameState::PlayerTurn) {
        throw std::runtime_error("Cannot double down at this time.");
    }

    if (!player_.double_down()) {
        throw std::runtime_error("Cannot double down due to insufficient chips or invalid hand.");
    }

    auto card_opt = deck_.draw_card();
    if (!card_opt) {
        throw std::runtime_error("Deck is empty. Cannot draw a card.");
    }

    player_.add_card(*card_opt);

    if (player_.active_hand().hand.is_busted()) {
        player_.set_hand_outcome(HandOutcome::Busted);
    }
    player_.stand(); // After doubling down, the player automatically stands

    player_.advance_to_next_hand();
}

void Game::split() {
    if (state_ != GameState::PlayerTurn) {
        throw std::runtime_error("Cannot split at this time.");
    }

    if (!player_.can_split()) {
        throw std::runtime_error("Cannot split the current hand.");
    }

    player_.split();
}

void Game::evaluate_hand(int hand_index) {
    const auto& player_hand = player_.hands()[hand_index].hand;
    if (player_hand.is_busted()) {
        player_.set_hand_outcome(HandOutcome::Busted);
        return;
    }

    if (state_ != GameState::RoundOver && state_ != GameState::DealerTurn) {
        return;
    }

    const auto& dealer_hand = dealer_.hand();

    bool is_natural_bj = player_hand.is_blackjack() && !player_.hands()[hand_index].is_from_split;

    if (is_natural_bj && !dealer_hand.is_blackjack()) {
        player_.set_hand_outcome(HandOutcome::Blackjack);
    } else if (!player_hand.is_blackjack() && dealer_hand.is_blackjack()) {
        player_.set_hand_outcome(HandOutcome::Lost);
    } else if (dealer_hand.is_busted()) {
        player_.set_hand_outcome(HandOutcome::Won);
    } else if (player_hand.value() > dealer_hand.value()) {
        player_.set_hand_outcome(HandOutcome::Won);
    } else if (player_hand.value() < dealer_hand.value()) {
        player_.set_hand_outcome(HandOutcome::Lost);
    } else {
        player_.set_hand_outcome(HandOutcome::Push);
    }
}

RoundSummary Game::finish_round() {
    int bet_sum = total_bet();
    int win_sum = payout();
    player_.add_chips(win_sum);
    state_ = GameState::RoundOver;
    events_.emplace_back(RoundOverEvent{.total_payout = win_sum});
    return RoundSummary{.total_bet = bet_sum, .total_payout = win_sum};
}

/// Win pays 1:1
/// Blackjack pays 3:2
/// Push returns the bet
/// No payout for lost or busted hands
///
/// To avoid runtime errors, don't call this when handoutcom is still pending
int Game::payout() {
    int total_payout = 0;
    for (const auto& hand : player_.hands()) {
        switch (hand.outcome) {
        case HandOutcome::Won:
            total_payout += hand.bet * 2;
            break;
        case HandOutcome::Blackjack:
            total_payout += static_cast<int>(hand.bet * 2.5);
            break;
        case HandOutcome::Push:
            total_payout += hand.bet;
            break;
        case HandOutcome::Lost:
        case HandOutcome::Busted:

            break;
        case HandOutcome::Pending:
            std::runtime_error("Cannot calculate payout for a hand that is still pending.");
        }
    }
    return total_payout;
}

} // namespace domain