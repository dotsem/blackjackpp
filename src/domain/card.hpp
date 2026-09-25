#pragma once

namespace domain {
    enum class Suit {
        Clubs,
        Diamonds,
        Hearts,
        Spades
    };
    enum class Rank {
        Two = 2,
        Three = 3,
        Four = 4,
        Five = 5,
        Six = 6,
        Seven = 7,
        Eight = 8,
        Nine = 9,
        Ten = 10,
        Jack = 11,
        Queen = 12,
        King = 13,
        Ace = 14
    };

    class Card {
    public:
        constexpr Card(Suit suit, Rank rank) noexcept
            : suit_(suit)
            , rank_(rank) {
        }

        bool operator==(const Card&) const = default;

        [[nodiscard]] constexpr Suit suit() const noexcept {
            return suit_;
        }

        [[nodiscard]] constexpr Rank rank() const noexcept {
            return rank_;
        }

        [[nodiscard]] constexpr int blackjack_value() const noexcept {
            if (rank_ >= Rank::Jack && rank_ <= Rank::King) {
                return 10;
            }
            if (rank_ == Rank::Ace) {
                return 11; // hand evaluator will handle the case where Ace can be 1 or 11
            }
            return static_cast<int>(rank_);
        }

    private:
        Suit suit_;
        Rank rank_;
    };
} // namespace domain