#pragma once
#include "SDL3/SDL_render.h"

#include <SDL3/SDL.h>
#include <memory>
#include <stdexcept>
namespace sdl {

class Renderer {
    struct Deleter {
        void operator()(SDL_Renderer* r) const noexcept {
            if (r)
                SDL_DestroyRenderer(r);
        }
    };
    using Ptr = std::unique_ptr<SDL_Renderer, Deleter>;

public:
    Renderer(SDL_Window* window, const char* name = nullptr)
        : handle_(SDL_CreateRenderer(window, name)) {
        if (!handle_)
            throw std::runtime_error(SDL_GetError());
    }

    [[nodiscard]] SDL_Renderer* get() const noexcept { return handle_.get(); }

private:
    Ptr handle_;
};
} // namespace sdl