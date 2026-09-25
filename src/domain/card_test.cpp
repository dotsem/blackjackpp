#include "domain/card.hpp"
#include <gtest/gtest.h>

using namespace domain;

TEST(CardTest, FaceCardEvaluateToTen) {
    Card jack{
        Suit::Hearts,
        Rank::Jack,
    };
    Card queen{ Suit::Diamonds, Rank::Queen };
    Card king{ Suit::Clubs, Rank::King };

    EXPECT_EQ(jack.blackjack_value(), 10);
    EXPECT_EQ(queen.blackjack_value(), 10);
    EXPECT_EQ(king.blackjack_value(), 10);
}

TEST(CardTest, AceEvaluatesToEleven) {
    Card ace{ Suit::Diamonds, Rank::Ace };

    EXPECT_EQ(ace.blackjack_value(), 11);
}

TEST(CardTest, NumberCardsEvaluateToTheirRank) {
    Card two{ Suit::Clubs, Rank::Two };
    Card three{ Suit::Hearts, Rank::Three };
    Card four{ Suit::Diamonds, Rank::Four };
    Card five{ Suit::Spades, Rank::Five };
    Card six{ Suit::Clubs, Rank::Six };
    Card seven{ Suit::Hearts, Rank::Seven };
    Card eight{ Suit::Diamonds, Rank::Eight };
    Card nine{ Suit::Spades, Rank::Nine };
    Card ten{ Suit::Clubs, Rank::Ten };

    EXPECT_EQ(two.blackjack_value(), 2);
    EXPECT_EQ(three.blackjack_value(), 3);
    EXPECT_EQ(four.blackjack_value(), 4);
    EXPECT_EQ(five.blackjack_value(), 5);
    EXPECT_EQ(six.blackjack_value(), 6);
    EXPECT_EQ(seven.blackjack_value(), 7);
    EXPECT_EQ(eight.blackjack_value(), 8);
    EXPECT_EQ(nine.blackjack_value(), 9);
    EXPECT_EQ(ten.blackjack_value(), 10);
}