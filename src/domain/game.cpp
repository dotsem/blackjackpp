#include "domain/event.hpp"
#include "game.hpp"
#include "hand.hpp"
#include <stdexcept>

namespace {
    constexpr int DealerStandThreshold = 17;
    constexpr double BlackjackPayoutMultiplier = 2.5;
}

namespace domain {

    void Game::deal_initial_cards() {
        if (state_ != GameState::Dealing) {
            throw std::runtime_error("Cannot deal cards at this time.");
        }

        for (int i = 0; i < 2; ++i) {
            deal_card_to_player_current_hand();
        }

        deal_card_to_dealer();
        deal_card_to_dealer(false);

        if (player_.active_hand().hand.is_blackjack()) {
            set_state(GameState::DealerTurn);
        }
    }

    void Game::deal_dealer_cards() {
        if (state_ != GameState::DealerTurn) {
            throw std::runtime_error("Cannot deal dealer cards at this time.");
        }

        dealer_.reveal_hole_card();
        events_.emplace_back(HoleCardRevealedEvent{ dealer_.hand().cards()[1].card });

        while ((dealer_.hand().is_soft() && dealer_.hand().value() == DealerStandThreshold) ||
               dealer_.hand().value() < DealerStandThreshold) {
            deal_card_to_dealer();
        }

        for (int i = 0; i < player_.hand_count(); ++i) {
            evaluate_hand(i);
        }
    }

    void Game::hit() {
        if (state_ != GameState::PlayerTurn) {
            throw std::runtime_error("Cannot hit at this time.");
        }

        deal_card_to_player_current_hand();

        if (player_.active_hand().hand.is_busted()) {
            player_.set_outcome_for_current_hand(HandOutcome::Busted);
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

        deal_card_to_player_current_hand();

        if (player_.active_hand().hand.is_busted()) {
            player_.set_outcome_for_current_hand(HandOutcome::Busted);
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

        size_t new_hand_idx = player_.active_hand_index() + 1;

        if (player_.hands().size() <= new_hand_idx) {
            throw std::runtime_error("New hand index is out of bounds after split.");
        }

        deal_card_to_player_current_hand();
        deal_card_to_player_hand(new_hand_idx);
    }

    void Game::evaluate_hand(int hand_index) {
        const auto& player_hand = player_.hands()[hand_index].hand;
        if (player_hand.is_busted()) {
            player_.set_outcome_for_hand(HandOutcome::Busted, hand_index);
            return;
        }

        if (state_ != GameState::RoundOver && state_ != GameState::DealerTurn) {
            return;
        }

        const auto& dealer_hand = dealer_.hand();

        bool is_natural_bj = player_hand.is_blackjack() && !player_.hands()[hand_index].is_from_split;

        if (is_natural_bj && !dealer_hand.is_blackjack()) {
            player_.set_outcome_for_hand(HandOutcome::Blackjack, hand_index);
        } else if ((player_hand.value() < dealer_hand.value()) || (!player_hand.is_blackjack() && dealer_hand.is_blackjack())) {
            player_.set_outcome_for_hand(HandOutcome::Lost, hand_index);
        } else if (dealer_hand.is_busted() || (player_hand.value() > dealer_hand.value())) {
            player_.set_outcome_for_hand(HandOutcome::Won, hand_index);
        } else {
            player_.set_outcome_for_hand(HandOutcome::Push, hand_index);
        }
    }

    RoundSummary Game::finish_round() {
        int bet_sum = total_bet();
        int win_sum = payout();
        player_.add_chips(win_sum);
        state_ = GameState::RoundOver;
        events_.emplace_back(RoundOverEvent{ .total_payout = win_sum });
        return RoundSummary{ .total_bet = bet_sum, .total_payout = win_sum };
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
                    total_payout += static_cast<int>(hand.bet * BlackjackPayoutMultiplier);
                    break;
                case HandOutcome::Push:
                    total_payout += hand.bet;
                    break;
                case HandOutcome::Lost:
                case HandOutcome::Busted:

                    break;
                case HandOutcome::Pending:
                    throw std::runtime_error("Cannot calculate payout for a hand that is still pending.");
            }
        }
        return total_payout;
    }

} // namespace domain