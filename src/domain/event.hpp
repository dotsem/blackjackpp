#pragma once

#include "domain/card.hpp"
#include <cstddef>
#include <variant>

namespace domain {

    struct CardDealtEvent {
        Card card;
        bool is_dealer{ false };
        size_t hand_index{ 0 };
        bool is_face_up{ true };
    };

    struct HandBustedEvent {
        size_t hand_index{ 0 };
    };

    struct HoleCardRevealedEvent {
        Card card;
    };

    struct RoundOverEvent {
        int total_payout{ 0 };
    };

    using GameEvent =
        std::variant<CardDealtEvent, HandBustedEvent, HoleCardRevealedEvent, RoundOverEvent>;
} // namespace domain