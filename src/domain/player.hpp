#pragma once
#include "domain/hand.hpp"

#include <sys/types.h>

namespace domain {

struct PlayerHand {
    Hand hand;
    int bet{0};
    bool is_settled{false}; // indicates if hand is finished against dealer
};

class Player {
public:
    void add_card(Card card) { current_hand().hand.add_card(card, true); }

    bool can_split() const noexcept {
        if (active_hand_index_ >= hands_.size())
            return false;
        const auto& h = current_hand();
        return h.hand.cards().size() == 2 &&
               h.hand.cards()[0].card.blackjack_value() ==
                   h.hand.cards()[1].card.blackjack_value() &&
               chips_ >= h.bet;
    }

    void split() {
        if (!can_split())
            return;

        int split_bet = current_hand().bet;
        Card card_to_move = current_hand().hand.pop();
        chips_ -= split_bet;
        PlayerHand new_hand;
        new_hand.bet = split_bet;
        new_hand.hand.add_card(card_to_move, true);
        hands_.push_back(new_hand);
    }

    bool can_double_down(u_int8_t hand_index) const noexcept {
        if (hand_index >= hands_.size())
            return false;
        const auto& h = hands_[hand_index];
        return chips_ >= h.bet;
    }

    bool double_down() {
        if (active_hand_index_ >= hands_.size())
            return false;
        PlayerHand* hand = &current_hand();
        if (chips_ < hand->bet)
            return false;
        chips_ -= hand->bet;
        hand->bet *= 2;

        // TODO: here the user should be prompted to draw one more card and then the hand is
        // automatically settled

        return true;
    }

    [[nodiscard]] const PlayerHand& active_hand() const noexcept { return current_hand(); }
    [[nodiscard]] const std::vector<PlayerHand>& hands() const noexcept { return hands_; }
    [[nodiscard]] u_int8_t active_hand_index() const noexcept { return active_hand_index_; }
    void advance_to_next_hand() noexcept {
        if (active_hand_index_ + 1 < hands_.size()) {
            ++active_hand_index_;
        }
    }
    [[nodiscard]] int chips() const noexcept { return chips_; }
    void add_chips(int amount) { chips_ += amount; }
    void remove_chips(int amount) { chips_ -= amount; }

private:
    std::vector<PlayerHand> hands_{1};
    u_int8_t active_hand_index_{0};
    int chips_{1000};

    [[nodiscard]] PlayerHand& current_hand() noexcept { return hands_[active_hand_index_]; }
    [[nodiscard]] const PlayerHand& current_hand() const noexcept {
        return hands_[active_hand_index_];
    }
};
} // namespace domain