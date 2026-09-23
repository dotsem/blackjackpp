#pragma once

#include "card.hpp"

#include <vector>

namespace domain {

struct DealtCard {
    Card card;
    bool is_face_up{true};
};

class Hand {
public:
    void add_card(Card& card, bool is_face_up = true) { cards_.push_back({card, is_face_up}); }

    void reveal_all() noexcept {
        for (auto& c : cards_)
            c.is_face_up = true;
    }
    void clear() { cards_.clear(); }

    [[nodiscard]] const std::vector<DealtCard>& cards() const noexcept { return cards_; }

    [[nodiscard]] int total(bool only_visible = false) const noexcept {
        int total = 0;
        int ace_count = 0;
        for (const auto& entry : cards_) {
            if (only_visible && !entry.is_face_up) {
                continue;
            }
            total += entry.card.blackjack_value();
            if (entry.card.rank() == Rank::Ace) {
                ++ace_count;
            }
        }
        while (total > 21 && ace_count > 0) {
            total -= 10;
            --ace_count;
        }
        return total;
    };

private:
    std::vector<DealtCard> cards_;
};
} // namespace domain