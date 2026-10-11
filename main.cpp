#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "app.hpp"
#include "presentation/layout.hpp"
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>

SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    try {
        if (!TTF_Init()) {
            throw std::runtime_error(SDL_GetError());
        }
        auto app = std::make_unique<App>("Blackjack++", presentation::layout::VirtualWidth, presentation::layout::VirtualHeight);
        *appstate = app.release();
        return SDL_APP_CONTINUE;
    } catch (const std::exception& e) {
        SDL_Log("Fatal init error: %s", e.what());
        return SDL_APP_FAILURE;
    }
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    return static_cast<App*>(appstate)->handle_event(*event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    return static_cast<App*>(appstate)->tick();
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result) {
    const std::unique_ptr<App> app(static_cast<App*>(appstate));
    TTF_Quit();
}