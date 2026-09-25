#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <stdexcept>
#include <string_view>

namespace sdl {

    class Window {
        struct Deleter {
            void operator()(SDL_Window* w) const noexcept {
                if (w != nullptr) {
                    SDL_DestroyWindow(w);
                }
            }
        };

        using Ptr = std::unique_ptr<SDL_Window, Deleter>;

    public:
        Window(std::string_view title, int width, int height, SDL_WindowFlags flags = 0)
            : handle_(SDL_CreateWindow(title.data(), width, height, flags)) {
            if (!handle_) {
                throw std::runtime_error(SDL_GetError());
            }
        }

        [[nodiscard]] SDL_Window* get() const noexcept {
            return handle_.get();
        }

    private:
        Ptr handle_;
    };
} // namespace sdl