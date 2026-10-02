#include "deck.hpp"
#include <gtest/gtest.h>

using namespace domain;

TEST(DeckTest, deck_has_52_cards) {
    Deck deck;
    EXPECT_EQ(deck.cards().size(), 52);
}

TEST(DeckTest, two_decks_have_104_cards) {
    Deck deck{ 2 };
    EXPECT_EQ(deck.cards().size(), 104);
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

TEST(DeckTest, reset_restores_deck_to_52_cards_and_clears_discards) {
    Deck deck;
    auto c1 = deck.draw_card();
    auto c2 = deck.draw_card();
    deck.discard_cards({ *c1, *c2 });

    EXPECT_EQ(deck.cards().size(), 50);
    EXPECT_EQ(deck.discarded_cards().size(), 2);

    deck.reset();

    EXPECT_EQ(deck.cards().size(), 52);
    EXPECT_EQ(deck.discarded_cards().size(), 0);
}

TEST(DeckTest, shuffle_changes_order_of_cards) {
    Deck deck1{ 1, false };
    Deck deck2{ 1, false };

    // Ensure both decks are in the same initial order
    EXPECT_EQ(deck1.cards(), deck2.cards());

    deck1.shuffle();

    // After shuffling, the order should be different
    EXPECT_NE(deck1.cards(), deck2.cards());
}

TEST(DeckTest, shuffle_is_truly_random) {
    Deck deck1;
    Deck deck2;

    // The moment this test fails, i'm betting on the lottery
    bool are_equal = true;
    for (int i = 0; i < 5; ++i) {
        deck1.shuffle();
        deck2.shuffle();
        if (deck1.cards() != deck2.cards()) {
            are_equal = false;
            break;
        }
    }

    EXPECT_FALSE(are_equal);
}

TEST(DeckTest, needs_reshuffle_returns_true_when_below_threshold) {
    Deck deck{ 1, false };

    EXPECT_FALSE(deck.needs_reshuffle());

    for (int i = 0; i < 42; ++i) {
        std::ignore = deck.draw_card();
    }
    EXPECT_TRUE(deck.needs_reshuffle());
}

TEST(DeckTest, reshuffle_discarded_into_deck_moves_discarded_cards_back_to_deck) {
    Deck deck;

    std::vector<Card> played_cards;
    for (int i = 0; i < 3; ++i) {
        played_cards.push_back(*deck.draw_card());
    }
    EXPECT_EQ(deck.cards().size(), 49);

    deck.discard_cards(played_cards);
    EXPECT_EQ(deck.discarded_cards().size(), 3);

    deck.reshuffle_discarded_into_deck();
    EXPECT_EQ(deck.discarded_cards().size(), 0);
    EXPECT_EQ(deck.cards().size(), 52);
}

TEST(DeckTest, discard_card_adds_single_card_to_discards) {
    Deck deck;
    auto card = deck.draw_card();
    ASSERT_TRUE(card.has_value());

    deck.discard_card(*card);
    EXPECT_EQ(deck.discarded_cards().size(), 1);
    EXPECT_EQ(deck.discarded_cards().front(), *card);
}