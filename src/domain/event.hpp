#pragma once

#include "card.hpp"
#include "game_state.hpp"
#include <cstddef>
#include <string>
#include <variant>

namespace domain {

    // lifecycle & shoe
    struct GameStateChangedEvent {
        GameState new_state;
    };

    struct DeckReshuffledEvent {
        size_t cards_remaining;
    };

    // betting & round start
    struct BetPlacedEvent {
        int bet_amount;
        int total_bet;
        int remaining_chips;
    };

    struct RoundStartedEvent {
        int initial_bet;
    };

    // dealing
    struct CardDealtEvent {
        Card card;
        bool is_dealer{ false };
        size_t hand_index{ 0 };
        bool is_face_up{ true };
    };

    // player turn actions
    struct ActiveHandChangedEvent {
        size_t previous_hand_index;
        size_t current_hand_index;
    };

    struct HandSplitEvent {
        size_t source_hand_index;
        size_t new_hand_index;
        int split_bet;
        int remaining_chips;
    };

    struct HandDoubledDownEvent {
        size_t hand_index;
        int added_bet;
        int total_hand_bet;
        int remaining_chips;
    };

    struct HandStoodEvent {
        size_t hand_index;
    };

    struct HandBustedEvent {
        size_t hand_index{ 0 };
    };

    // dealer turn actions
    struct DealerTurnStartedEvent {};

    struct HoleCardRevealedEvent {
        Card card;
    };

    // round resolution
    struct RoundOverEvent {
        int total_payout{ 0 };
        int final_chips{ 0 };
        std::string summary;
    };

    using GameEvent =
        std::variant<
            GameStateChangedEvent,
            DeckReshuffledEvent,
            BetPlacedEvent,
            RoundStartedEvent,
            CardDealtEvent,
            ActiveHandChangedEvent,
            HandSplitEvent,
            HandDoubledDownEvent,
            HandStoodEvent,
            HandBustedEvent,
            DealerTurnStartedEvent,
            HoleCardRevealedEvent,
            RoundOverEvent>;
} // namespace domain