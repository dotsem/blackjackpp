#include "game_test.hpp"
#include "card.hpp"
#include "dealer.hpp"
#include "deck.hpp"
#include "game.hpp"
#include "player.hpp"
#include <gtest/gtest.h>

using namespace domain;

TEST(GameTest, constructor_initializes_with_empty_hands_and_full_deck) {
    Game game = test_helpers::create_game();

    EXPECT_EQ(game.player().active_hand().hand.size(), 0);
    EXPECT_EQ(game.dealer().hand().cards().size(), 0);
    EXPECT_EQ(game.deck().cards().size(), 52);
    EXPECT_EQ(game.deck().discarded_cards().size(), 0);
}

TEST(GameTest, total_bet_calculates_from_all_hands) {
    Game game = test_helpers::create_split_game();

    EXPECT_EQ(game.total_bet(), 200);
}

TEST(GameTest, start_round_initializes_game_state_and_deals_cards) {
    std::vector<Card> cards = {
        Card{ Suit::Spades, Rank::Ten },
        Card{ Suit::Spades, Rank::Eight },
        Card{ Suit::Hearts, Rank::Nine },
        Card{ Suit::Hearts, Rank::Seven },
    };
    Game game = test_helpers::create_game_with_deck(std::move(cards), 1000);
    game.start_round(200);
    EXPECT_EQ(game.player().active_hand().bet, 200);
    EXPECT_EQ(game.player().chips(), 800);
    EXPECT_EQ(game.state(), GameState::PlayerTurn);
    EXPECT_EQ(game.player().active_hand().hand.size(), 2);
    EXPECT_EQ(game.dealer().hand().cards().size(), 2);
}

TEST(GameTest, start_round_throws_for_invalid_amount) {
    Game game = test_helpers::create_game(1000);

    EXPECT_THROW(game.start_round(-100), std::runtime_error);
    EXPECT_THROW(game.start_round(0), std::runtime_error);
    EXPECT_THROW(game.start_round(2000), std::runtime_error);
}

TEST(GameTest, start_round_throws_if_not_waiting_for_bets) {
    Game game = test_helpers::create_game(1000);
    game.set_state(GameState::Dealing);

    EXPECT_THROW(game.start_round(100), std::runtime_error);
}

TEST(GameTest, set_state_changes_game_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::PlayerTurn);

    EXPECT_EQ(game.state(), GameState::PlayerTurn);
}

TEST(GameTest, deal_initial_cards_throws_if_not_in_dealing_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::PlayerTurn);

    EXPECT_THROW(game.deal_initial_cards(), std::runtime_error);
}

TEST(GameTest, start_round_reshuffles_discards_when_needed) {
    Deck deck{ 1, false };
    std::vector<Card> discards;
    for (int i = 0; i < 45; ++i) {
        discards.push_back(*deck.draw_card());
    }
    deck.discard_cards(discards);
    EXPECT_TRUE(deck.needs_reshuffle());
    EXPECT_EQ(deck.discarded_cards().size(), 45);

    Game game(Player(1000), Dealer{}, std::move(deck));
    game.start_round(100);

    EXPECT_EQ(game.deck().discarded_cards().size(), 0);
    EXPECT_EQ(game.deck().cards().size(), 52 - 4);
    EXPECT_EQ(game.player().active_hand().bet, 100);
}

TEST(GameTest, discard_cards_moves_all_table_cards_to_deck_discards) {
    Player player;
    player.add_card_to_current_hand({ Suit::Hearts, Rank::Ten });
    player.add_card_to_current_hand({ Suit::Diamonds, Rank::Queen });

    Dealer dealer;
    dealer.add_upcard({ Suit::Spades, Rank::Ten });
    dealer.add_upcard({ Suit::Clubs, Rank::Eight });

    Game game(player, dealer, Deck{});
    game.discard_cards();

    EXPECT_TRUE(game.player().active_hand().hand.is_empty());
    EXPECT_TRUE(game.dealer().hand().cards().empty());
    EXPECT_EQ(game.deck().discarded_cards().size(), 4);
}

TEST(GameTest, finish_round_calculates_payout_and_retains_cards_on_table) {
    Player player;
    player.add_card_to_current_hand({ Suit::Hearts, Rank::Ten });
    player.add_card_to_current_hand({ Suit::Diamonds, Rank::Queen });
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({ Suit::Spades, Rank::Ten });
    dealer.add_upcard({ Suit::Clubs, Rank::Eight });

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);
    game.evaluate_hand(0);

    RoundSummary summary = game.finish_round();

    EXPECT_EQ(summary.total_bet, 100);
    EXPECT_EQ(summary.total_payout, 200);
    EXPECT_EQ(game.state(), GameState::RoundOver);
    EXPECT_EQ(game.player().active_hand().hand.size(), 2);
    EXPECT_EQ(game.dealer().hand().cards().size(), 2);
    EXPECT_EQ(game.deck().discarded_cards().size(), 0);
}

TEST(GameTest, finish_round_throws_if_any_hand_is_pending) {
    Game game = test_helpers::create_split_game();

    game.set_state(GameState::PlayerTurn);

    while (!game.player().hands()[0].hand.is_busted()) {
        game.hit();
    }

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Busted);
    EXPECT_EQ(game.player().hands()[1].outcome, HandOutcome::Pending);
    EXPECT_THROW(game.finish_round(), std::runtime_error);
}

TEST(GameTest, reset_game_resets_hands_state_and_deck_discards) {
    Player player;
    player.add_card_to_current_hand({ Suit::Hearts, Rank::Ten });

    Dealer dealer;
    dealer.add_upcard({ Suit::Spades, Rank::Ten });

    Deck deck;
    auto c = deck.draw_card();
    deck.discard_card(*c);

    Game game(player, dealer, std::move(deck));
    game.set_state(GameState::PlayerTurn);

    game.reset_game();

    EXPECT_EQ(game.state(), GameState::WaitingForBets);
    EXPECT_FALSE(game.result().has_value());
    EXPECT_TRUE(game.player().active_hand().hand.is_empty());
    EXPECT_TRUE(game.dealer().hand().cards().empty());
    EXPECT_EQ(game.deck().cards().size(), 52);
    EXPECT_EQ(game.deck().discarded_cards().size(), 0);
}
