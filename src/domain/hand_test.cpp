#include "domain/card.hpp"
#include "domain/hand.hpp"

#include <gtest/gtest.h>

using namespace domain;

TEST(HandTest, add_card_increases_size) {
    Hand hand;
    Card card{Suit::Hearts, Rank::Ace};
    hand.add_card(card);
    EXPECT_EQ(hand.cards().size(), 1);
}

TEST(HandTest, reveal_all_sets_all_cards_face_up) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::King};
    hand.add_card(card1, false);
    hand.add_card(card2, false);
    hand.reveal_all();
    for (const auto& dealt_card : hand.cards()) {
        EXPECT_TRUE(dealt_card.is_face_up);
    }
}

TEST(HandTest, clear_removes_all_cards) {
    Hand hand;
    Card card{Suit::Hearts, Rank::Ace};
    hand.add_card(card);
    hand.clear();
    EXPECT_EQ(hand.cards().size(), 0);
}

TEST(HandTest, pop_removes_and_returns_last_card) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::King};
    hand.add_card(card1);
    hand.add_card(card2);
    Card popped_card = hand.pop();
    EXPECT_EQ(popped_card, card2);
    EXPECT_EQ(hand.cards().size(), 1);
}

TEST(HandTest, pop_throws_if_hand_is_empty) {
    Hand hand;
    EXPECT_THROW(std::ignore = hand.pop(), std::runtime_error);
}

TEST(HandTest, value_calculates_correctly_with_aces) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::Six};
    hand.add_card(card1);
    hand.add_card(card2);
    EXPECT_EQ(hand.value(), 17);

    Card card3{Suit::Clubs, Rank::Ace};
    hand.add_card(card3);

    // one Ace counts as 11, the other as 1
    EXPECT_EQ(hand.value(), 18);
}

TEST(HandTest, value_only_calculates_visible_cards) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::Six};
    hand.add_card(card1, true);
    hand.add_card(card2, false);

    // only the Ace is visible
    EXPECT_EQ(hand.value(true), 11);
}

TEST(HandTest, is_blackjack_returns_true_for_blackjack) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::King};
    hand.add_card(card1);
    hand.add_card(card2);
    EXPECT_TRUE(hand.is_blackjack());
}

TEST(HandTest, is_blackjack_returns_false_for_non_blackjack) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::Nine};
    hand.add_card(card1);
    hand.add_card(card2);
    EXPECT_FALSE(hand.is_blackjack());
}

TEST(HandTest, is_soft_returns_true_for_soft_hand) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::Six};
    hand.add_card(card1);
    hand.add_card(card2);
    EXPECT_TRUE(hand.is_soft());
}

TEST(HandTest, is_soft_returns_false_for_hard_hand) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ten};
    Card card2{Suit::Diamonds, Rank::Seven};
    hand.add_card(card1);
    hand.add_card(card2);
    EXPECT_FALSE(hand.is_soft());
}

TEST(HandTest, is_empty_correctly_identifies_empty_hand) {
    Hand hand;
    EXPECT_TRUE(hand.is_empty());

    Card card{Suit::Hearts, Rank::Ace};
    hand.add_card(card);
    EXPECT_FALSE(hand.is_empty());
}

TEST(HandTest, size_returns_correct_number_of_cards) {
    Hand hand;
    EXPECT_EQ(hand.size(), 0);

    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::King};
    Card card3{Suit::Clubs, Rank::Queen};
    hand.add_card(card1);
    hand.add_card(card2);
    hand.add_card(card3);
    EXPECT_EQ(hand.size(), 3);
}

TEST(HandTest, has_visible_cards_returns_true_if_any_card_is_face_up) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::King};
    hand.add_card(card1, false);
    hand.add_card(card2, true);
    EXPECT_TRUE(hand.has_visible_cards());

    std::ignore = hand.pop();
    EXPECT_FALSE(hand.has_visible_cards());
}

TEST(HandTest, has_hidden_cards_returns_true_if_any_cards_are_face_down) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ace};
    Card card2{Suit::Diamonds, Rank::King};
    hand.add_card(card1, false);
    hand.add_card(card2, true);
    EXPECT_TRUE(hand.has_hidden_cards());

    hand.reveal_all();
    EXPECT_FALSE(hand.has_hidden_cards());
}

TEST(HandTest, is_busted_returns_true_if_value_exceeds_21) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ten};
    Card card2{Suit::Diamonds, Rank::King};
    Card card3{Suit::Clubs, Rank::Two};
    hand.add_card(card1);
    hand.add_card(card2);
    hand.add_card(card3);
    EXPECT_TRUE(hand.is_busted());
}

TEST(HandTest, is_busted_returns_false_if_value_is_21_or_less) {
    Hand hand;
    Card card1{Suit::Hearts, Rank::Ten};
    Card card2{Suit::Diamonds, Rank::Seven};
    hand.add_card(card1);
    hand.add_card(card2);
    EXPECT_FALSE(hand.is_busted());
}