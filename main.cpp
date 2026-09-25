/* primitives.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some lines,
 * rectangles and points to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#include "SDL_fur_coat/renderer.hpp"
#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL_fur_coat/window.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct AppState {
    sdl::Window window;
    sdl::Renderer renderer;
    SDL_FPoint points[500]{};

    AppState(std::string_view title, int w, int h)
        : window(title, w, h, SDL_WINDOW_RESIZABLE)
        , renderer(window.get()) {
    }
};

SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    SDL_SetAppMetadata("Example Renderer Primitives", "1.0", "com.example.renderer-primitives");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    try {
        auto* app = new AppState("examples/renderer/primitives", 640, 480);
        SDL_SetRenderLogicalPresentation(app->renderer.get(), 640, 480,
            SDL_LOGICAL_PRESENTATION_LETTERBOX);
        for (auto& point : app->points) {
            point.x = (SDL_randf() * 440.0F) + 100.0F;
            point.y = (SDL_randf() * 280.0F) + 100.0F;
        }
        *appstate = app;
        return SDL_APP_CONTINUE;
    } catch (const std::exception& e) {
        SDL_Log("Failed initialization: %s", e.what());
        return SDL_APP_FAILURE;
    }
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* app = static_cast<AppState*>(appstate);
    SDL_Renderer* renderer = app->renderer.get();

    SDL_FRect rect;

    SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    rect.x = rect.y = 100;
    rect.w = 440;
    rect.h = 280;
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPoints(renderer, app->points, SDL_arraysize(app->points));

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    rect.x += 30;
    rect.y += 30;
    rect.w -= 60;
    rect.h -= 60;
    SDL_RenderRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, 0, 0, 640, 480);
    SDL_RenderLine(renderer, 0, 480, 640, 0);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result) {
    delete static_cast<AppState*>(appstate);
    SDL_Quit();
}
