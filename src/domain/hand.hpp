#pragma once

#include "card.hpp"

#include <stdexcept>
#include <vector>

namespace domain {

struct DealtCard {
    Card card;
    bool is_face_up{true};
};

class Hand {
public:
    void add_card(Card card, bool is_face_up = true) { cards_.push_back({card, is_face_up}); }

    void reveal_all() noexcept {
        for (auto& c : cards_)
            c.is_face_up = true;
    }
    void clear() { cards_.clear(); }

    [[nodiscard]] const std::vector<DealtCard>& cards() const noexcept { return cards_; }

    [[nodiscard]] Card pop() {
        if (cards_.empty()) {
            throw std::runtime_error("Hand is empty, cannot pop card.");
        }
        DealtCard dealt_card = cards_.back();
        cards_.pop_back();
        return dealt_card.card;
    }

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

    [[nodiscard]] bool is_blackjack() const noexcept { return cards_.size() == 2 && total() == 21; }

    [[nodiscard]] bool is_soft() const noexcept {
        int total = 0;
        int ace_count = 0;
        for (const auto& entry : cards_) {
            if (!entry.is_face_up)
                continue;
            total += entry.card.blackjack_value();
            if (entry.card.rank() == Rank::Ace)
                ++ace_count;
        }
        while (total > 21 && ace_count > 0) {
            total -= 10;
            --ace_count;
        }
        // if any Ace is still counted as 11, the hand is soft
        return ace_count > 0;
    }

    [[nodiscard]] bool is_empty() const noexcept { return cards_.empty(); }
    [[nodiscard]] size_t size() const noexcept { return cards_.size(); }
    [[nodiscard]] bool has_visible_cards() const noexcept {
        for (const auto& entry : cards_) {
            if (entry.is_face_up) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool has_hidden_cards() const noexcept {
        for (const auto& entry : cards_) {
            if (!entry.is_face_up) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool is_busted() const noexcept { return total() > 21; }

private:
    std::vector<DealtCard> cards_;
};
} // namespace domain