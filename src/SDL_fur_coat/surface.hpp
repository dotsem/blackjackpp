#pragma once

#include "SDL_fur_coat/resource.hpp"
#include <SDL3/SDL_surface.h>

namespace sdl {

    using SurfaceHandle = UniqueResource<SDL_Surface, SDL_DestroySurface>;

    class Surface {
    public:
        Surface() = default;

        explicit Surface(SDL_Surface* raw_surface)
            : handle_(raw_surface) {
        }

        [[nodiscard]] SDL_Surface* get() const noexcept {
            return handle_.get();
        }

        [[nodiscard]] explicit operator bool() const noexcept {
            return handle_ != nullptr;
        }

        [[nodiscard]] int width() const noexcept {
            return handle_ ? handle_->w : 0;
        }

        [[nodiscard]] int height() const noexcept {
            return handle_ ? handle_->h : 0;
        }

        void reset(SDL_Surface* raw = nullptr) noexcept {
            handle_.reset(raw);
        }

    private:
        SurfaceHandle handle_;
    };

} // namespace sdl