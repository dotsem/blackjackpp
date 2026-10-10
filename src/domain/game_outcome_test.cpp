#include "game.hpp"
#include "gtest/gtest.h"

using namespace domain;

namespace {
    HandOutcome evaluate_scenario(
        const std::vector<Card>& player_cards,
        const std::vector<Card>& dealer_cards,
        bool is_from_split = false) {
        Player player;
        for (const auto& card : player_cards) {
            player.add_card_to_current_hand(card);
        }
        player.active_hand().bet = 100;
        player.active_hand().is_from_split = is_from_split;

        Dealer dealer;
        for (const auto& card : dealer_cards) {
            dealer.add_upcard(card);
        }

        Game game(player, dealer, Deck{});
        game.set_state(GameState::DealerTurn);
        game.evaluate_hand(0);
        return game.player().active_hand().outcome;
    }
}

// player: 23 -> busted
// dealer: 17 -> won
TEST(GameOutcomeTest, player_busted_remains_busted) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ten }, { Suit::Spades, Rank::Eight }, { Suit::Clubs, Rank::Five } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Seven } }),
        HandOutcome::Busted);
}

// player: 21 (natural) -> blackjack
// dealer: 19 -> lost
TEST(GameOutcomeTest, natural_blackjack_wins_over_regular_dealer_hand) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ace }, { Suit::Spades, Rank::King } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Nine } }),
        HandOutcome::Blackjack);
}

// player: 21 (natural) -> push
// dealer: 21 (natural) -> push
TEST(GameOutcomeTest, natural_blackjack_pushes_against_dealer_blackjack) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ace }, { Suit::Spades, Rank::King } },
            { { Suit::Diamonds, Rank::Ace }, { Suit::Clubs, Rank::Queen } }),
        HandOutcome::Push);
}

// player: 20 -> lost
// dealer: 21 (natural) -> won
TEST(GameOutcomeTest, dealer_blackjack_beats_player_twenty) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ten }, { Suit::Spades, Rank::Ten } },
            { { Suit::Diamonds, Rank::Ace }, { Suit::Clubs, Rank::King } }),
        HandOutcome::Lost);
}

// player: 21 (multi-card) -> lost
// dealer: 21 (natural) -> won
TEST(GameOutcomeTest, dealer_blackjack_beats_multi_card_twenty_one) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Seven }, { Suit::Spades, Rank::Seven }, { Suit::Clubs, Rank::Seven } },
            { { Suit::Diamonds, Rank::Ace }, { Suit::Clubs, Rank::King } }),
        HandOutcome::Lost);
}

// player: 18 -> won
// dealer: 22 -> busted
TEST(GameOutcomeTest, dealer_busted_awards_player_win) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ten }, { Suit::Spades, Rank::Eight } },
            { { Suit::Clubs, Rank::Two }, { Suit::Diamonds, Rank::Ten }, { Suit::Spades, Rank::Ten } }),
        HandOutcome::Won);
}

// player: 20 -> won
// dealer: 19 -> lost
TEST(GameOutcomeTest, player_higher_value_wins) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ten }, { Suit::Spades, Rank::Ten } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Nine } }),
        HandOutcome::Won);
}

// player: 18 -> lost
// dealer: 19 -> won
TEST(GameOutcomeTest, player_lower_value_loses) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ten }, { Suit::Spades, Rank::Eight } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Nine } }),
        HandOutcome::Lost);
}

// player: 19 -> push
// dealer: 19 -> push
TEST(GameOutcomeTest, equal_value_pushes) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ten }, { Suit::Spades, Rank::Nine } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Nine } }),
        HandOutcome::Push);
}

// player: 21 (split) -> won
// dealer: 19 -> lost
TEST(GameOutcomeTest, split_twenty_one_is_regular_win_not_natural_blackjack) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ace }, { Suit::Spades, Rank::King } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Nine } },
            /*is_from_split=*/true),
        HandOutcome::Won);
}

// player: 21 (split) -> push
// dealer: 21 -> push
TEST(GameOutcomeTest, split_twenty_one_pushes_with_dealer_twenty_one) {
    EXPECT_EQ(
        evaluate_scenario(
            { { Suit::Hearts, Rank::Ace }, { Suit::Spades, Rank::King } },
            { { Suit::Diamonds, Rank::Ten }, { Suit::Clubs, Rank::Six }, { Suit::Clubs, Rank::Five } },
            /*is_from_split=*/true),
        HandOutcome::Push);
}