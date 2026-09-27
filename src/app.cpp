
#include "app.hpp"
#include "SDL3/SDL_keycode.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

App::App(std::string_view title, int width, int height)
    : window_(title, width, height, SDL_WINDOW_RESIZABLE)
    , renderer_(window_.get())
    , last_ticks_(SDL_GetTicksNS())
    , game_(domain::Game(domain::Player{ 1000 }, domain::Dealer{}, domain::Deck{})) {
    SDL_SetRenderLogicalPresentation(renderer_.get(), width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

SDL_AppResult App::handle_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.key) {
                case SDLK_ESCAPE:
                    return SDL_APP_SUCCESS;
                case SDLK_SPACE:
                    game_.hit();
                    break;
                case SDLK_RETURN:
                    game_.stand();
                    break;
                case SDLK_D:
                    game_.double_down();
                    break;
                case SDLK_S:
                    game_.split();
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult App::tick() {
    if (!is_running_) {
        return SDL_APP_SUCCESS;
    }

    const auto now = SDL_GetTicksNS();
    [[maybe_unused]] const auto dt = static_cast<float>(now - last_ticks_) / 1'000'000'000.0F;
    last_ticks_ = now;

    SDL_SetRenderDrawColor(renderer_.get(), 18, 18, 18, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_.get());
    SDL_RenderPresent(renderer_.get());

    return SDL_APP_CONTINUE;
}
