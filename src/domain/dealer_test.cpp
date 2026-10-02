#include "dealer.hpp"
#include <gtest/gtest.h>

using namespace domain;

TEST(DealerTest, add_upcard_adds_face_up_card_to_hand) {
    Dealer dealer;
    Card card{ Suit::Hearts, Rank::Ace };
    dealer.add_upcard(card);

    EXPECT_EQ(dealer.hand().cards().size(), 1);
    EXPECT_EQ(dealer.hand().cards()[0].card, card);
    EXPECT_TRUE(dealer.hand().cards()[0].is_face_up);
}

TEST(DealerTest, add_hole_card_adds_face_down_card_to_hand) {
    Dealer dealer;
    Card card{ Suit::Diamonds, Rank::King };
    dealer.add_hole_card(card);

    EXPECT_EQ(dealer.hand().cards().size(), 1);
    EXPECT_EQ(dealer.hand().cards()[0].card, card);
    EXPECT_FALSE(dealer.hand().cards()[0].is_face_up);
}

TEST(DealerTest, discard_hand_removes_cards_and_returns_them) {
    Dealer dealer;
    Card upcard{ Suit::Hearts, Rank::Ace };
    Card hole_card{ Suit::Diamonds, Rank::King };
    dealer.add_upcard(upcard);
    dealer.add_hole_card(hole_card);

    auto discarded = dealer.discard_hand();

    EXPECT_TRUE(dealer.hand().cards().empty());
    ASSERT_EQ(discarded.size(), 2);
    EXPECT_EQ(discarded[0], upcard);
    EXPECT_EQ(discarded[1], hole_card);
}

TEST(DealerTest, clear_hand_resets_hand) {
    Dealer dealer;
    dealer.add_upcard({ Suit::Hearts, Rank::Ace });
    dealer.clear_hand();

    EXPECT_TRUE(dealer.hand().cards().empty());
}