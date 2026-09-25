#pragma once

#include "domain/hand.hpp"

namespace domain {
    class Dealer {
    public:
        void add_upcard(Card card) {
            hand_.add_card(card, true);
        }

        void add_hole_card(Card card) {
            hand_.add_card(card, false);
        }

        void reveal_hole_card() {
            hand_.reveal_all();
        }

        [[nodiscard]] const Hand& hand() const noexcept {
            return hand_;
        }

        void clear_hand() {
            hand_.clear();
        }

    private:
        // dealer has only one hand
        Hand hand_;
    };
} // namespace domain
