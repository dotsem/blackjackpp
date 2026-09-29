#pragma once

#include <SDL3/SDL_init.h>
union SDL_Event;

namespace domain {
    class Game;
}

namespace presentation {

    class View {
    public:
        View() = default;
        virtual ~View() = default;
        View(const View&) = default;
        View& operator=(const View&) = default;
        View(View&&) = default;
        View& operator=(View&&) = default;

        [[nodiscard]] virtual SDL_AppResult tick(float dt) = 0;
        [[nodiscard]] virtual SDL_AppResult handle_event(const SDL_Event& event) = 0;
    };
}