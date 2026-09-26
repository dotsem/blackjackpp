#pragma once

#include "SDL_fur_coat/resource.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdexcept>

namespace sdl {

    using RendererHandle = UniqueResource<SDL_Renderer, SDL_DestroyRenderer>;

    class Renderer {
    public:
        Renderer(SDL_Window* window, const char* name = nullptr)
            : handle_(SDL_CreateRenderer(window, name)) {
            if (!handle_) {
                throw std::runtime_error(SDL_GetError());
            }
        }

        [[nodiscard]] SDL_Renderer* get() const noexcept {
            return handle_.get();
        }

    private:
        RendererHandle handle_;
    };

} // namespace sdl