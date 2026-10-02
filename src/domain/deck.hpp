#pragma once

#include "card.hpp"
#include <algorithm>
#include <optional>
#include <random>
#include <vector>

namespace {
    constexpr int SingleDeckSize = 52;
    constexpr float ReshuffleThreshold = 0.20F;
}

namespace domain {
    class Deck {
    public:
        Deck(int num_decks = 1, bool shuffle_on_init = true)
            : num_decks_(num_decks) {
            fill_deck();
            if (shuffle_on_init) {
                shuffle();
            }
        }

        explicit Deck(std::vector<Card> cards)
            : cards_(std::move(cards)) {
        }

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

        void shuffle() {
            std::shuffle(cards_.begin(), cards_.end(), rng_);
        }

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

        [[nodiscard]] const std::vector<Card>& cards() const noexcept {
            return cards_;
        }

        [[nodiscard]] bool needs_reshuffle() const noexcept {
            return static_cast<float>(cards_.size()) / static_cast<float>(SingleDeckSize * num_decks_) < ReshuffleThreshold;
        }

    private:
        std::vector<Card> cards_;
        int num_decks_{ 1 };
        std::mt19937 rng_{ std::random_device{}() };

        void fill_deck() {
            cards_.reserve(static_cast<long>(SingleDeckSize * num_decks_));
            for (int d = 0; d < num_decks_; ++d) {
                for (int s = static_cast<int>(Suit::Diamonds); s <= static_cast<int>(Suit::Clubs); ++s) {
                    for (int r = static_cast<int>(Rank::Two); r <= static_cast<int>(Rank::Ace); ++r) {
                        cards_.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
                    }
                }
            }
        }
    };
} // namespace domain