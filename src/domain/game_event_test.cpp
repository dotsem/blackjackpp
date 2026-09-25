#include "domain/game.hpp"
#include "game_test.hpp"

#include "gtest/gtest.h"

using namespace domain;

TEST(GameEventTest, deal_initial_cards_emits_four_deal_events) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

    auto events = game.poll_events();
    ASSERT_EQ(events.size(), 4);

    ASSERT_TRUE(std::holds_alternative<CardDealtEvent>(events[0]));
    const auto& e0 = std::get<CardDealtEvent>(events[0]);
    EXPECT_FALSE(e0.is_dealer);
    EXPECT_TRUE(e0.is_face_up);

    const auto& e3 = std::get<CardDealtEvent>(events[3]);
    EXPECT_TRUE(e3.is_dealer);
    EXPECT_FALSE(e3.is_face_up);

    EXPECT_TRUE(game.poll_events().empty());
}

TEST(GameEventTest, deal_dealer_cards_emits_hole_card_revealed_event) {
    Game game = test_helpers::create_game();
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

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
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

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
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

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
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

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
    game.place_bet(100);
    game.set_state(GameState::Dealing);
    game.deal_initial_cards();

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
