#include "game.hpp"
#include "game_test.hpp"
#include "gtest/gtest.h"

using namespace domain;

TEST(GameEventTest, deal_initial_cards_emits_four_deal_events) {
    Game game = test_helpers::create_game();
    game.start_round(100);

    auto events = game.poll_events();
    std::vector<CardDealtEvent> deal_events;
    for (const auto& ev : events) {
        if (const auto* d = std::get_if<CardDealtEvent>(&ev)) {
            deal_events.push_back(*d);
        }
    }
    ASSERT_EQ(deal_events.size(), 4);

    EXPECT_FALSE(deal_events[0].is_dealer);
    EXPECT_TRUE(deal_events[0].is_face_up);

    EXPECT_TRUE(deal_events[3].is_dealer);
    EXPECT_FALSE(deal_events[3].is_face_up);

    EXPECT_TRUE(game.poll_events().empty());
}

TEST(GameEventTest, deal_dealer_cards_emits_hole_card_revealed_event) {
    Game game = test_helpers::create_game();
    game.start_round(100);

    game.set_state(GameState::DealerTurn);
    game.deal_dealer_cards();

    auto events = game.poll_events();
    bool hole_card_revealed_event_found = false;
    for (const auto& event : events) {
        if (std::holds_alternative<HoleCardRevealedEvent>(event)) {
            hole_card_revealed_event_found = true;
            break;
        }
    }
    EXPECT_TRUE(hole_card_revealed_event_found);
}

TEST(GameEventTest, hit_emits_card_dealt_event) {
    Game game = test_helpers::create_game();
    game.start_round(100);

    game.set_state(GameState::PlayerTurn);
    game.hit();

    auto events = game.poll_events();
    bool card_dealt_event_found = false;
    for (const auto& event : events) {
        if (std::holds_alternative<CardDealtEvent>(event)) {
            card_dealt_event_found = true;
            break;
        }
    }
    EXPECT_TRUE(card_dealt_event_found);
}

TEST(GameEventTest, hit_emits_hand_busted_event_if_player_busts) {
    Game game = test_helpers::create_game();
    game.start_round(100);

    game.set_state(GameState::PlayerTurn);

    // Force the player to bust by drawing cards until they bust
    while (!game.player().active_hand().hand.is_busted()) {
        game.hit();
    }

    auto events = game.poll_events();
    bool hand_busted_event_found = false;
    for (const auto& event : events) {
        if (std::holds_alternative<HandBustedEvent>(event)) {
            hand_busted_event_found = true;
            break;
        }
    }
    EXPECT_TRUE(hand_busted_event_found);
}

TEST(GameEventTest, finish_round_emits_round_over_event) {
    Game game = test_helpers::create_game();
    game.start_round(100);

    game.set_state(GameState::DealerTurn);
    game.deal_dealer_cards();

    game.finish_round();

    auto events = game.poll_events();
    bool round_over_event_found = false;
    for (const auto& event : events) {
        if (std::holds_alternative<RoundOverEvent>(event)) {
            round_over_event_found = true;
            break;
        }
    }
    EXPECT_TRUE(round_over_event_found);
}

TEST(GameEventTest, poll_events_drains_events) {
    Game game = test_helpers::create_game();
    game.start_round(100);

    auto events1 = game.poll_events();
    EXPECT_FALSE(events1.empty());

    auto events2 = game.poll_events();
    EXPECT_TRUE(events2.empty());
}

TEST(GameEventTest, poll_events_returns_empty_vector_if_no_events) {
    Game game = test_helpers::create_game();
    auto events = game.poll_events();
    EXPECT_TRUE(events.empty());
}

TEST(GameEventTest, start_round_emits_bet_placed_and_round_started_events) {
    Game game = test_helpers::create_game();
    game.start_round(50);

    auto events = game.poll_events();
    bool bet_placed_found = false;
    bool round_started_found = false;

    for (const auto& event : events) {
        if (const auto* bp = std::get_if<BetPlacedEvent>(&event)) {
            bet_placed_found = true;
            EXPECT_EQ(bp->bet_amount, 50);
            EXPECT_EQ(bp->total_bet, 50);
        } else if (const auto* rs = std::get_if<RoundStartedEvent>(&event)) {
            round_started_found = true;
            EXPECT_EQ(rs->initial_bet, 50);
        }
    }

    EXPECT_TRUE(bet_placed_found);
    EXPECT_TRUE(round_started_found);
}

TEST(GameEventTest, stand_emits_hand_stood_event) {
    Game game = test_helpers::create_game();
    game.start_round(100);
    game.set_state(GameState::PlayerTurn);
    static_cast<void>(game.poll_events());

    game.stand();

    auto events = game.poll_events();
    bool hand_stood_found = false;
    for (const auto& event : events) {
        if (std::holds_alternative<HandStoodEvent>(event)) {
            hand_stood_found = true;
            break;
        }
    }
    EXPECT_TRUE(hand_stood_found);
}

TEST(GameEventTest, dealer_turn_started_emits_event) {
    Game game = test_helpers::create_game();
    game.start_round(100);
    game.set_state(GameState::DealerTurn);
    static_cast<void>(game.poll_events());

    game.deal_dealer_cards();

    auto events = game.poll_events();
    bool dealer_turn_found = false;
    for (const auto& event : events) {
        if (std::holds_alternative<DealerTurnStartedEvent>(event)) {
            dealer_turn_found = true;
            break;
        }
    }
    EXPECT_TRUE(dealer_turn_found);
}
