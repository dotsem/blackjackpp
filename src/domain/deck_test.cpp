#include "domain/deck.hpp"
#include <gtest/gtest.h>

using namespace domain;

TEST(DeckTest, deck_has_52_cards) {
    Deck deck;
    EXPECT_EQ(deck.cards().size(), 52);
}

TEST(DeckTest, draw_card_reduces_deck_size) {
    Deck deck;
    auto card_opt = deck.draw_card();
    EXPECT_TRUE(card_opt.has_value());
    EXPECT_EQ(deck.cards().size(), 51);
}

TEST(DeckTest, draw_specific_card_removes_card_from_deck) {
    Deck deck;
    auto card_opt = deck.draw_specific_card(Suit::Hearts, Rank::Ace);
    EXPECT_TRUE(card_opt.has_value());
    EXPECT_EQ(card_opt->suit(), Suit::Hearts);
    EXPECT_EQ(card_opt->rank(), Rank::Ace);
    EXPECT_EQ(deck.cards().size(), 51);
}

TEST(DeckTest, draw_specific_card_returns_nullopt_if_card_not_found) {
    Deck deck;
    // draw the Ace of Hearts first
    auto card_opt = deck.draw_specific_card(Suit::Hearts, Rank::Ace);
    EXPECT_TRUE(card_opt.has_value());

    // now try to draw the Ace of Hearts again
    auto card_opt2 = deck.draw_specific_card(Suit::Hearts, Rank::Ace);
    EXPECT_FALSE(card_opt2.has_value());
}

TEST(DeckTest, reset_restores_deck_to_52_cards) {
    Deck deck;
    std::ignore = deck.draw_card();
    std::ignore = deck.draw_card();
    EXPECT_EQ(deck.cards().size(), 50);
    deck.reset();
    EXPECT_EQ(deck.cards().size(), 52);
}

TEST(DeckTest, shuffle_changes_order_of_cards) {
    Deck deck1;
    Deck deck2;

    // Ensure both decks are in the same initial order
    EXPECT_EQ(deck1.cards(), deck2.cards());

    deck1.shuffle();

    // After shuffling, the order should be different
    EXPECT_NE(deck1.cards(), deck2.cards());
}