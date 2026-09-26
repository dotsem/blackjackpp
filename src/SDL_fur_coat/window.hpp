#pragma once

#include "SDL_fur_coat/resource.hpp"
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string_view>

namespace sdl {

    using WindowHandle = UniqueResource<SDL_Window, SDL_DestroyWindow>;

    class Window {
    public:
        Window(std::string_view title, int width, int height, SDL_WindowFlags flags = 0)
            : handle_(SDL_CreateWindow(std::string(title).c_str(), width, height, flags)) {
            if (!handle_) {
                throw std::runtime_error(SDL_GetError());
            }
        }

        [[nodiscard]] SDL_Window* get() const noexcept {
            return handle_.get();
        }

    private:
        WindowHandle handle_;
    };

} // namespace sdl