
#include "app.hpp"
#include "presentation/assets/texture_manager.hpp"
#include "presentation/views/table_view.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

App::App(std::string_view title, int width, int height)
    : game_(domain::Game(domain::Player(), domain::Dealer(), domain::Deck()))
    , window_(title, width, height, SDL_WINDOW_RESIZABLE)
    , renderer_(window_.get())
    , textures_(renderer_.get())
    , current_view_(new presentation::TableView(renderer_.get(), textures_, &game_))
    , last_ticks_(SDL_GetTicksNS()) {
    SDL_SetRenderLogicalPresentation(renderer_.get(), width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

SDL_AppResult App::handle_event(const SDL_Event& event) {
    try {
        return current_view_->handle_event(event);
    } catch (const std::exception& e) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Domain action rejected: %s", e.what());
        return SDL_APP_CONTINUE;
    }
}

SDL_AppResult App::tick() {
    if (!is_running_) {
        return SDL_APP_SUCCESS;
    }

    const auto now = SDL_GetTicksNS();
    [[maybe_unused]] const auto dt = static_cast<float>(now - last_ticks_) / 1'000'000'000.0F;
    last_ticks_ = now;

    return current_view_->tick(dt);
}
