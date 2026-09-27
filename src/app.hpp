

#include "SDL3/SDL_init.h"
#include "SDL_fur_coat/renderer.hpp"
#include "SDL_fur_coat/window.hpp"
#include "domain/game.hpp"

// why: avoid large SDL_Event.h header to be included
union SDL_Event;

class App {
public:
    explicit App(std::string_view title, int width, int height);
    ~App() = default;

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    [[nodiscard]] SDL_AppResult handle_event(const SDL_Event& event);
    [[nodiscard]] SDL_AppResult tick();

private:
    sdl::Window window_;
    sdl::Renderer renderer_;
    uint64_t last_ticks_{ 0 };
    bool is_running_{ true };
    domain::Game game_;
};