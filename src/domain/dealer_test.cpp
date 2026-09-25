#include "domain/dealer.hpp"

#include <gtest/gtest.h>

using namespace domain;

TEST(DealerTest, add_upcard_adds_face_up_card_to_hand) {
    Dealer dealer;
    Card card{Suit::Hearts, Rank::Ace};
    dealer.add_upcard(card);

    EXPECT_EQ(dealer.hand().cards().size(), 1);
    EXPECT_EQ(dealer.hand().cards()[0].card, card);
    EXPECT_TRUE(dealer.hand().cards()[0].is_face_up);
}

TEST(DealerTest, add_hole_card_adds_face_down_card_to_hand) {
    Dealer dealer;
    Card card{Suit::Diamonds, Rank::King};
    dealer.add_hole_card(card);

    EXPECT_EQ(dealer.hand().cards().size(), 1);
    EXPECT_EQ(dealer.hand().cards()[0].card, card);
    EXPECT_FALSE(dealer.hand().cards()[0].is_face_up);
}