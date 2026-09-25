#include "domain/card.hpp"
#include "domain/player.hpp"

#include <gtest/gtest.h>

using namespace domain;

TEST(PlayerTest, constructor_initializes_with_empty_hand) {
    Player player;

    EXPECT_EQ(player.hands().size(), 1);
    EXPECT_TRUE(player.active_hand().hand.is_empty());
}

TEST(PlayerTest, add_card_to_current_hand_adds_card_to_active_hand) {
    Player player;
    Card card{Suit::Hearts, Rank::Ace};
    player.add_card_to_current_hand(card);

    EXPECT_EQ(player.active_hand().hand.cards().size(), 1);
    EXPECT_EQ(player.active_hand().hand.cards()[0].card, card);
}

TEST(PlayerTest, clear_hands_resets_hands_and_active_hand_index) {
    Player player;
    Card card{Suit::Hearts, Rank::Ace};
    player.add_card_to_current_hand(card);
    player.clear_hands();

    EXPECT_EQ(player.hands().size(), 1);
    EXPECT_TRUE(player.active_hand().hand.is_empty());
    EXPECT_EQ(player.active_hand_index(), 0);
}

TEST(PlayerTest, can_split_returns_true_for_valid_split) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Eight};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.active_hand().bet = 100;

    EXPECT_TRUE(player.can_split());
}

TEST(PlayerTest, can_split_returns_false_for_invalid_split) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Hearts, Rank::Nine};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.active_hand().bet = 100;

    EXPECT_FALSE(player.can_split());
}

TEST(PlayerTest, split_creates_new_hand_and_moves_card) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Eight};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.active_hand().bet = 100;
    player.split();

    EXPECT_EQ(player.hands().size(), 2);
    EXPECT_EQ(player.hands()[0].hand.cards().size(), 1);
    EXPECT_EQ(player.hands()[1].hand.cards().size(), 1);
    EXPECT_EQ(player.hands()[0].hand.cards()[0].card, card1);
    EXPECT_EQ(player.hands()[1].hand.cards()[0].card, card2);
}

TEST(PlayerTest, invalid_split_does_not_change_hands) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Nine};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.active_hand().bet = 100;
    player.split();

    EXPECT_EQ(player.hands().size(), 1);
    EXPECT_EQ(player.hands()[0].hand.cards().size(), 2);
}

TEST(PlayerTest, can_double_down_returns_true_for_valid_double_down) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Three};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.active_hand().bet = 100;

    EXPECT_TRUE(player.can_double_down());
}

TEST(PlayerTest, can_double_down_returns_false_for_invalid_double_down) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    player.add_card_to_current_hand(card1);
    player.active_hand().bet = 100;

    EXPECT_FALSE(player.can_double_down());
}

TEST(PlayerTest, can_double_down_returns_false_for_insufficient_chips) {
    Player player = Player(100);
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Three};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.remove_chips(70);
    player.active_hand().bet = 70;

    EXPECT_FALSE(player.can_double_down());
}

TEST(PlayerTest, double_down_doubles_the_bet) {
    Player player(200);
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Three};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.remove_chips(100);
    player.active_hand().bet = 100;
    bool can_double_down = player.double_down();

    EXPECT_TRUE(can_double_down);
    EXPECT_EQ(player.active_hand().bet, 200);
    EXPECT_EQ(player.chips(), 0);
}

TEST(PlayerTest, double_down_fails_for_invalid_conditions) {
    Player player = Player(100);
    Card card1{Suit::Hearts, Rank::Eight};
    player.add_card_to_current_hand(card1);
    player.remove_chips(70);
    player.active_hand().bet = 70;
    bool can_double_down = player.double_down();

    EXPECT_FALSE(can_double_down);
    EXPECT_EQ(player.active_hand().bet, 70);
    EXPECT_EQ(player.chips(), 30);
}

TEST(PlayerTest, stand_sets_active_hand_stand_to_true) {
    Player player;

    EXPECT_FALSE(player.active_hand_stands());
    player.stand();
    EXPECT_TRUE(player.active_hand_stands());
}

TEST(PlayerTest, advance_to_next_hand_does_not_wrap_around) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Eight};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.split();

    EXPECT_EQ(player.active_hand_index(), 0);
    player.advance_to_next_hand();
    EXPECT_EQ(player.active_hand_index(), 1);
    player.advance_to_next_hand();
    EXPECT_EQ(player.active_hand_index(), 1);
}

TEST(PlayerTest, set_outcome_for_hand_sets_correct_outcome) {
    Player player;
    player.set_outcome_for_hand(HandOutcome::Busted, 0);

    EXPECT_EQ(player.hands()[0].outcome, HandOutcome::Busted);
}

TEST(PlayerTest, set_outcome_for_current_hand_sets_correct_outcome) {
    Player player;
    player.set_outcome_for_current_hand(HandOutcome::Blackjack);

    EXPECT_EQ(player.hands()[0].outcome, HandOutcome::Blackjack);
}

TEST(PlayerTest, all_hands_stands_returns_true_when_all_hands_stand) {
    Player player;
    player.stand();

    EXPECT_TRUE(player.all_hands_stands());
}

TEST(PlayerTest, all_hands_settled_returns_true_when_all_hands_have_outcome) {
    Player player;
    player.set_outcome_for_current_hand(HandOutcome::Blackjack);

    EXPECT_TRUE(player.all_hands_settled());
}

TEST(PlayerTest, add_chips_increases_chip_count) {
    Player player(100);
    player.add_chips(50);

    EXPECT_EQ(player.chips(), 150);
}

TEST(PlayerTest, remove_chips_decreases_chip_count) {
    Player player(100);
    player.remove_chips(30);

    EXPECT_EQ(player.chips(), 70);
}
