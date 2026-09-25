#pragma once

#include "card.hpp"

#include <algorithm>
#include <optional>
#include <random>
#include <vector>

namespace domain {
class Deck {
public:
    Deck() { fill_deck(); }

    void reset() {
        cards_.clear();
        fill_deck();
    }

    [[nodiscard]] std::optional<Card> draw_card() {
        if (cards_.empty()) {
            return std::nullopt;
        }
        Card card = cards_.back();
        cards_.pop_back();
        return card;
    }

    void shuffle() { std::shuffle(cards_.begin(), cards_.end(), rng_); }

    // Only for testing purposes, allows drawing a specific card from the deck
    [[nodiscard]] std::optional<Card> draw_specific_card(Suit suit, Rank rank) {
        for (auto it = cards_.begin(); it != cards_.end(); ++it) {
            if (it->suit() == suit && it->rank() == rank) {
                Card card = *it;
                cards_.erase(it);
                return card;
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] const std::vector<Card>& cards() const noexcept { return cards_; }

private:
    std::vector<Card> cards_;
    std::mt19937 rng_{std::random_device{}()};

    void fill_deck() {
        cards_.reserve(52);
        for (int s = static_cast<int>(Suit::Clubs); s <= static_cast<int>(Suit::Spades); ++s) {
            for (int r = static_cast<int>(Rank::Two); r <= static_cast<int>(Rank::Ace); ++r) {
                cards_.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
            }
        }
    }
};
} // namespace domain