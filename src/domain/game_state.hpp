#pragma once

#include <cstdint>

namespace domain {
    enum class GameState : std::uint8_t {
        WaitingForBets,
        Dealing,
        PlayerTurn,
        DealerTurn,
        RoundOver
    };
}