#pragma once
#include "domain/hand.hpp"
#include <vector>

namespace domain {

    enum class HandOutcome {
        Pending,
        Busted,
        Won,
        Lost,
        Push,
        Blackjack
    };

    struct PlayerHand {
        Hand hand;
        int bet{ 0 };
        bool stand{ false };
        bool is_from_split{ false };
        HandOutcome outcome{ HandOutcome::Pending };
    };

    class Player {
    public:
        Player(int chips = 1000)
            : chips_(chips) {
            hands_.reserve(4);
        }

        void clear_hands() {
            hands_.clear();
            hands_.reserve(4);
            hands_.emplace_back();
            active_hand_index_ = 0;
        }

        void add_card_to_current_hand(Card card) {
            current_hand().hand.add_card(card, true);
        }

        void add_card_to_hand(Card card, size_t hand_index) {
            if (hand_index >= hands_.size()) {
                throw std::runtime_error("Hand index out of bounds.");
            }
            hands_[hand_index].hand.add_card(card);
        }

        [[nodiscard]] bool can_split() const noexcept {
            if (active_hand_index_ >= hands_.size()) {
                return false;
            }
            const auto& h = current_hand();
            return h.hand.cards().size() == 2 &&
                   h.hand.cards()[0].card.blackjack_value() ==
                       h.hand.cards()[1].card.blackjack_value() &&
                   chips_ >= h.bet;
        }

        void split() {
            if (!can_split()) {
                return;
            }

            int split_bet = current_hand().bet;
            Card card_to_move = current_hand().hand.pop();
            chips_ -= split_bet;
            PlayerHand new_hand;
            new_hand.bet = split_bet;
            new_hand.hand.add_card(card_to_move, true);
            new_hand.is_from_split = true;
            hands_.push_back(new_hand);
        }

        [[nodiscard]] bool can_double_down() const noexcept {
            if (active_hand_index_ >= hands_.size()) {
                return false;
            }

            const auto& h = current_hand();
            if (h.hand.size() != 2) {
                return false;
            }

            return chips_ >= h.bet;
        }

        bool double_down() {
            if (!can_double_down()) {
                return false;
            }

            auto& h = current_hand();

            chips_ -= h.bet;
            h.bet *= 2;

            return true;
        }

        [[nodiscard]] const PlayerHand& active_hand() const noexcept {
            return current_hand();
        }

        [[nodiscard]] PlayerHand& active_hand() noexcept {
            return current_hand();
        }

        [[nodiscard]] const std::vector<PlayerHand>& hands() const noexcept {
            return hands_;
        }

        [[nodiscard]] bool active_hand_stands() const noexcept {
            return current_hand().stand;
        }

        void stand() {
            current_hand().stand = true;
        }

        [[nodiscard]] HandOutcome hand_outcome() const noexcept {
            return current_hand().outcome;
        }

        void set_outcome_for_hand(HandOutcome outcome, int hand_index) {
            hands_[hand_index].outcome = outcome;
        }

        void set_outcome_for_current_hand(HandOutcome outcome) {
            current_hand().outcome = outcome;
        }

        [[nodiscard]] size_t hand_count() const noexcept {
            return hands_.size();
        }

        [[nodiscard]] size_t active_hand_index() const noexcept {
            return active_hand_index_;
        }

        void advance_to_next_hand() noexcept {
            if (active_hand_index_ + 1 < hands_.size()) {
                ++active_hand_index_;
            }
        }

        [[nodiscard]] bool all_hands_stands() const noexcept {
            for (const auto& hand : hands_) {
                if (!hand.stand) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] bool all_hands_settled() const noexcept {
            for (const auto& hand : hands_) {
                if (hand.outcome == HandOutcome::Pending) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] int chips() const noexcept {
            return chips_;
        }

        void add_chips(int amount) {
            chips_ += amount;
        }

        void remove_chips(int amount) {
            chips_ -= amount;
        }

    private:
        std::vector<PlayerHand> hands_{ PlayerHand{} };
        size_t active_hand_index_{ 0 };
        int chips_;

        [[nodiscard]] PlayerHand& current_hand() noexcept {
            return hands_[active_hand_index_];
        }

        [[nodiscard]] const PlayerHand& current_hand() const noexcept {
            return hands_[active_hand_index_];
        }
    };
} // namespace domain