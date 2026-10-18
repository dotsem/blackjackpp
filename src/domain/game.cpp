#include "game.hpp"
#include "event.hpp"
#include "hand.hpp"
#include <cstddef>
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
            deal_dealer_cards();
            finish_round();
        } else {
            set_state(GameState::PlayerTurn);
        }
    }

    void Game::deal_dealer_cards() {
        if (state_ != GameState::DealerTurn) {
            throw std::runtime_error("Cannot deal dealer cards at this time.");
        }

        events_.emplace_back(DealerTurnStartedEvent{});
        dealer_.reveal_hole_card();
        events_.emplace_back(HoleCardRevealedEvent{ dealer_.hand().cards().at(1).card });

        while ((dealer_.hand().is_soft() && dealer_.hand().value() == DealerStandThreshold) ||
               dealer_.hand().value() < DealerStandThreshold) {
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

        deal_card_to_player_current_hand();

        if (player_.active_hand().hand.is_busted()) {
            player_.set_outcome_for_current_hand(HandOutcome::Busted);
            size_t prev_index = player_.active_hand_index();
            events_.emplace_back(HandBustedEvent{ .hand_index = prev_index });
            if (!player_.advance_to_next_hand()) {
                finish_round();
            } else {
                events_.emplace_back(ActiveHandChangedEvent{
                    .previous_hand_index = prev_index,
                    .current_hand_index = player_.active_hand_index() });
            }
        }
    }

    void Game::stand() {
        if (state_ != GameState::PlayerTurn) {
            throw std::runtime_error("Cannot stand at this time.");
        }

        size_t prev_index = player_.active_hand_index();
        events_.emplace_back(HandStoodEvent{ .hand_index = prev_index });
        player_.stand();
        if (!player_.advance_to_next_hand()) {
            set_state(GameState::DealerTurn);
            deal_dealer_cards();
            finish_round();
        } else {
            events_.emplace_back(ActiveHandChangedEvent{
                .previous_hand_index = prev_index,
                .current_hand_index = player_.active_hand_index() });
        }
    }

    void Game::double_down() {
        if (state_ != GameState::PlayerTurn) {
            throw std::runtime_error("Cannot double down at this time.");
        }

        size_t hand_idx = player_.active_hand_index();
        int added_bet = player_.active_hand().bet;

        if (!player_.double_down()) {
            throw std::runtime_error("Cannot double down due to insufficient chips or invalid hand.");
        }

        events_.emplace_back(HandDoubledDownEvent{
            .hand_index = hand_idx,
            .added_bet = added_bet,
            .total_hand_bet = player_.active_hand().bet,
            .remaining_chips = player_.chips() });

        deal_card_to_player_current_hand();

        if (player_.active_hand().hand.is_busted()) {
            player_.set_outcome_for_current_hand(HandOutcome::Busted);
            events_.emplace_back(HandBustedEvent{ .hand_index = hand_idx });
        }
        events_.emplace_back(HandStoodEvent{ .hand_index = hand_idx });
        player_.stand(); // After doubling down, the player automatically stands

        if (!player_.advance_to_next_hand()) {
            set_state(GameState::DealerTurn);
            deal_dealer_cards();
            finish_round();
        } else {
            events_.emplace_back(ActiveHandChangedEvent{
                .previous_hand_index = hand_idx,
                .current_hand_index = player_.active_hand_index() });
        }
    }

    void Game::split() {
        if (state_ != GameState::PlayerTurn) {
            throw std::runtime_error("Cannot split at this time.");
        }

        if (!player_.can_split()) {
            throw std::runtime_error("Cannot split the current hand.");
        }

        size_t src_hand_idx = player_.active_hand_index();
        int split_bet = player_.active_hand().bet;

        player_.split();

        size_t new_hand_idx = player_.hands().size() - 1;

        events_.emplace_back(HandSplitEvent{
            .source_hand_index = src_hand_idx,
            .new_hand_index = new_hand_idx,
            .split_bet = split_bet,
            .remaining_chips = player_.chips() });

        deal_card_to_player_current_hand();
        deal_card_to_player_hand(new_hand_idx);
    }

    void Game::evaluate_hand(size_t hand_index) {
        const auto& player_hand = player_.hands().at(hand_index).hand;
        if (player_hand.is_busted()) {
            player_.set_outcome_for_hand(HandOutcome::Busted, hand_index);
            return;
        }

        if (state_ != GameState::RoundOver && state_ != GameState::DealerTurn) {
            return;
        }

        const auto& dealer_hand = dealer_.hand();

        bool is_natural_bj = player_hand.is_blackjack() && !player_.hands().at(hand_index).is_from_split;

        if (dealer_hand.is_blackjack()) {
            player_.set_outcome_for_hand(is_natural_bj ? HandOutcome::Push : HandOutcome::Lost, hand_index);
            return;
        }

        if (is_natural_bj) {
            player_.set_outcome_for_hand(HandOutcome::Blackjack, hand_index);
            return;
        }

        if (dealer_hand.is_busted() || (player_hand.value() > dealer_hand.value())) {
            player_.set_outcome_for_hand(HandOutcome::Won, hand_index);
        } else if (player_hand.value() < dealer_hand.value()) {
            player_.set_outcome_for_hand(HandOutcome::Lost, hand_index);
        } else {
            player_.set_outcome_for_hand(HandOutcome::Push, hand_index);
        }
    }

    void Game::finish_round() {
        int bet_sum = total_bet();
        int win_sum = payout();
        player_.add_chips(win_sum);
        round_summary_ = RoundSummary{ .total_bet = bet_sum, .total_payout = win_sum };
        set_state(GameState::RoundOver);
        events_.emplace_back(RoundOverEvent{
            .total_payout = win_sum,
            .final_chips = player_.chips(),
            .summary = round_summary_.is_profit() ? "Profit" : (win_sum == bet_sum ? "Push" : "Loss") });
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