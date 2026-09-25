#include "domain/game_test.hpp"

#include "domain/card.hpp"
#include "domain/dealer.hpp"
#include "domain/deck.hpp"
#include "domain/game.hpp"
#include "domain/player.hpp"

#include <gtest/gtest.h>

using namespace domain;

TEST(GameTest, constructor_initializes_with_empty_hands_and_full_deck) {
    Game game = test_helpers::create_game();

    EXPECT_EQ(game.player().active_hand().hand.size(), 0);
    EXPECT_EQ(game.dealer().hand().cards().size(), 0);
    EXPECT_EQ(game.deck().cards().size(), 52);
}

TEST(GameTest, total_bet_calculates_from_all_hands) {
    Game game = test_helpers::create_split_game();

    EXPECT_EQ(game.total_bet(), 200);
}

TEST(GameTest, place_bet_reduces_player_chips_and_sets_bet) {
    Game game = test_helpers::create_game(1000);
    game.place_bet(200);

    EXPECT_EQ(game.player().active_hand().bet, 200);
    EXPECT_EQ(game.player().chips(), 800);
}

TEST(GameTest, place_bet_throws_for_invalid_amount) {
    Game game = test_helpers::create_game(1000);

    EXPECT_THROW(game.place_bet(-100), std::runtime_error);
    EXPECT_THROW(game.place_bet(0), std::runtime_error);
    EXPECT_THROW(game.place_bet(2000), std::runtime_error);
}

TEST(GameTest, place_bet_throws_if_not_waiting_for_bets) {
    Game game = test_helpers::create_game(1000);
    game.set_state(GameState::Dealing);

    EXPECT_THROW(game.place_bet(100), std::runtime_error);
}

TEST(GameTest, set_state_changes_game_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::PlayerTurn);

    EXPECT_EQ(game.state(), GameState::PlayerTurn);
}

TEST(GameTest, deal_initial_cards_deals_two_cards_to_player_and_two_cards_to_dealer) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    EXPECT_EQ(game.player().active_hand().hand.size(), 2);
    EXPECT_FALSE(game.player().active_hand().hand.has_hidden_cards());
    EXPECT_TRUE(game.dealer().hand().has_visible_cards());

    EXPECT_EQ(game.dealer().hand().cards().size(), 2);
    EXPECT_TRUE(game.dealer().hand().has_hidden_cards());
    EXPECT_TRUE(game.dealer().hand().has_visible_cards());
}

TEST(GameTest, deal_initial_cards_throws_if_not_in_dealing_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::PlayerTurn);

    EXPECT_THROW(game.deal_initial_cards(), std::runtime_error);
}

TEST(GameState, deal_dealer_cards_deals_until_17_or_higher) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    game.set_state(GameState::DealerTurn);
    game.deal_dealer_cards();

    EXPECT_GE(game.dealer().hand().value(), 17);
    EXPECT_FALSE(game.dealer().hand().is_soft());
    EXPECT_FALSE(game.dealer().hand().has_hidden_cards());
}

TEST(GameState, deal_dealer_cards_throws_if_not_in_dealer_turn_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::PlayerTurn);

    EXPECT_THROW(game.deal_dealer_cards(), std::runtime_error);
}

TEST(GameState, hit_deals_card_to_player_and_advances_if_busted) {
    Game game = test_helpers::create_split_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    game.set_state(GameState::PlayerTurn);

    while (!game.player().hands()[0].hand.is_busted()) {
        game.hit();
    }

    EXPECT_TRUE(game.player().hands()[0].hand.is_busted());
    EXPECT_FALSE(game.player().hands()[1].hand.is_busted());
    EXPECT_EQ(game.player().active_hand_index(), 1);
}

TEST(GameState, hit_throws_if_not_in_player_turn_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::DealerTurn);

    EXPECT_THROW(game.hit(), std::runtime_error);
}

TEST(GameState, stand_sets_active_hand_stand_and_advances) {
    Game game = test_helpers::create_split_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    game.set_state(GameState::PlayerTurn);
    game.stand();

    EXPECT_TRUE(game.player().hands()[0].stand);
    EXPECT_EQ(game.player().active_hand_index(), 1);
}

TEST(GameState, stand_throws_if_not_in_player_turn_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::DealerTurn);

    EXPECT_THROW(game.stand(), std::runtime_error);
}

TEST(GameState, double_down_doubles_bet_and_advances) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    game.set_state(GameState::PlayerTurn);
    int initial_bet = game.player().active_hand().bet;
    int initial_chips = game.player().chips();
    game.double_down();

    EXPECT_EQ(game.player().active_hand().bet, initial_bet * 2);
    EXPECT_EQ(game.player().chips(), initial_chips - initial_bet);
    EXPECT_TRUE(game.player().active_hand_stands());
}

TEST(GameState, double_down_throws_if_not_in_player_turn_state) {
    Game game = test_helpers::create_game();
    game.set_state(GameState::DealerTurn);

    EXPECT_THROW(game.double_down(), std::runtime_error);
}

TEST(GameState, double_down_throws_if_cannot_double_down) {
    Game game = test_helpers::create_game(100);
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    game.set_state(GameState::PlayerTurn);

    EXPECT_THROW(game.double_down(), std::runtime_error);
}

TEST(GameState, split_splits_hand) {
    Player player;
    Card card1{Suit::Hearts, Rank::Eight};
    Card card2{Suit::Diamonds, Rank::Eight};
    player.add_card_to_current_hand(card1);
    player.add_card_to_current_hand(card2);
    player.active_hand().bet = 100;
    Dealer dealer;
    Deck deck;
    Game game(player, dealer, deck);

    game.set_state(GameState::PlayerTurn);
    game.split();

    EXPECT_EQ(game.player().hands().size(), 2);
    EXPECT_EQ(game.player().active_hand_index(), 0);
    EXPECT_EQ(game.player().hands()[0].hand.cards().size(), 2);
    EXPECT_EQ(game.player().hands()[1].hand.cards().size(), 2);
}

TEST(GameState, split_throws_if_not_in_player_turn_state) {
    Game game = test_helpers::create_split_game();
    game.set_state(GameState::DealerTurn);

    EXPECT_THROW(game.split(), std::runtime_error);
}

TEST(GameState, split_throws_if_cannot_split) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    game.set_state(GameState::PlayerTurn);

    EXPECT_THROW(game.split(), std::runtime_error);
}

TEST(GameState, evaluate_hand_sets_outcome_for_busted_hand) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();
    game.set_state(GameState::PlayerTurn);

    while (!game.player().active_hand().hand.is_busted()) {
        game.hit();
    }

    game.evaluate_hand(0);

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Busted);
}

TEST(GameState, evaluate_hand_sets_outcome_for_winning_hand) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Ten});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::Queen});
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({Suit::Spades, Rank::Ten});
    dealer.add_upcard({Suit::Clubs, Rank::Eight});

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);

    game.evaluate_hand(0);

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Won);
}

TEST(GameState, evaluate_hand_sets_outcome_for_losing_hand) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Ten});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::Eight});
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({Suit::Spades, Rank::Ten});
    dealer.add_upcard({Suit::Clubs, Rank::Queen});

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);

    game.evaluate_hand(0);

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Lost);
}

TEST(GameState, evaluate_hand_sets_outcome_for_push) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Ten});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::Eight});
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({Suit::Spades, Rank::Ten});
    dealer.add_upcard({Suit::Clubs, Rank::Eight});

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);

    game.evaluate_hand(0);

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Push);
}

TEST(GameState, evaluate_hand_sets_outcome_for_blackjack) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Ace});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::King});
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({Suit::Spades, Rank::Ten});
    dealer.add_upcard({Suit::Clubs, Rank::Eight});

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);

    game.evaluate_hand(0);

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Blackjack);
}

TEST(GameState, evaluate_hand_sets_outcome_for_blackjack_push) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Ace});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::King});
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({Suit::Spades, Rank::Ace});
    dealer.add_upcard({Suit::Clubs, Rank::King});

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);

    game.evaluate_hand(0);

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Push);
}

TEST(GameState, evaluate_hand_does_nothing_if_not_in_dealer_turn_or_round_over) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Two});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::Three});
    player.active_hand().bet = 100;
    Game game(player, Dealer{}, Deck{});
    game.set_state(GameState::PlayerTurn);
    game.evaluate_hand(0);
    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Pending);
}

TEST(GameState, finish_round_calculates_total_bet_and_payout) {
    Player player;
    player.add_card_to_current_hand({Suit::Hearts, Rank::Ten});
    player.add_card_to_current_hand({Suit::Diamonds, Rank::Queen});
    player.active_hand().bet = 100;

    Dealer dealer;
    dealer.add_upcard({Suit::Spades, Rank::Ten});
    dealer.add_upcard({Suit::Clubs, Rank::Eight});

    Game game(player, dealer, Deck{});
    game.set_state(GameState::DealerTurn);
    game.evaluate_hand(0);

    RoundSummary summary = game.finish_round();

    EXPECT_EQ(summary.total_bet, 100);
    EXPECT_EQ(summary.total_payout, 200);
}

TEST(GameState, finish_round_throws_if_any_hand_is_pending) {
    Game game = test_helpers::create_split_game();

    game.set_state(GameState::PlayerTurn);

    while (!game.player().hands()[0].hand.is_busted()) {
        game.hit();
    }

    EXPECT_EQ(game.player().hands()[0].outcome, HandOutcome::Busted);
    EXPECT_EQ(game.player().hands()[1].outcome, HandOutcome::Pending);
    EXPECT_THROW(game.finish_round(), std::runtime_error);
}
