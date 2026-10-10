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
            discarded_cards_.clear();
            fill_deck();
            shuffle();
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

        [[nodiscard]] const std::vector<Card>& discarded_cards() const noexcept {
            return discarded_cards_;
        }

        [[nodiscard]] int num_decks() const noexcept {
            return num_decks_;
        }

        [[nodiscard]] bool needs_reshuffle() const noexcept {
            return static_cast<float>(cards_.size()) / static_cast<float>(SingleDeckSize * num_decks_) < ReshuffleThreshold;
        }

        void discard_cards(const std::vector<Card>& cards) {
            discarded_cards_.insert(discarded_cards_.end(), cards.begin(), cards.end());
        }

        void discard_card(const Card& card) {
            discarded_cards_.push_back(card);
        }

        void reshuffle_discarded_into_deck() {
            cards_.insert(cards_.end(), discarded_cards_.begin(), discarded_cards_.end());
            discarded_cards_.clear();
            shuffle();
        }

    private:
        std::vector<Card> cards_;
        std::vector<Card> discarded_cards_;
        int num_decks_{ 1 };
        std::mt19937 rng_{ std::random_device{}() };

        void fill_deck() {
            cards_.reserve(static_cast<long>(SingleDeckSize * num_decks_));
            for (int d = 0; d < num_decks_; ++d) {
                for (int s = 0; s < static_cast<int>(Suit::Count); ++s) {
                    for (int r = static_cast<int>(Rank::Ace); r < static_cast<int>(Rank::Count); ++r) {
                        cards_.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
                    }
                }
            }
        }
    };
} // namespace domain