#pragma once

#include "SDL_fur_coat/renderer.hpp"
#include "SDL_fur_coat/resource.hpp"
#include "SDL_fur_coat/surface.hpp"
#include <SDL3/SDL_render.h>
#include <stdexcept>

namespace sdl {

    using TextureHandle = UniqueResource<SDL_Texture, SDL_DestroyTexture>;

    class Texture {
    public:
        Texture() = default;

        explicit Texture(SDL_Texture* raw_texture)
            : handle_(raw_texture) {
            update_dimensions();
        }

        static Texture from_surface(SDL_Renderer* renderer, SDL_Surface* surface) {
            if (renderer == nullptr || surface == nullptr) {
                throw std::invalid_argument("Renderer and surface must not be null.");
            }
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
            if (tex == nullptr) {
                throw std::runtime_error(SDL_GetError());
            }
            return Texture(tex);
        }

        static Texture from_surface(const Renderer& r, SDL_Surface* s) {
            return from_surface(r.get(), s);
        }

        static Texture from_surface(SDL_Renderer* r, const Surface& s) {
            return from_surface(r, s.get());
        }

        static Texture from_surface(const Renderer& r, const Surface& s) {
            return from_surface(r.get(), s.get());
        }

        [[nodiscard]] SDL_Texture* get() const noexcept {
            return handle_.get();
        }

        [[nodiscard]] float width() const noexcept {
            return width_;
        }

        [[nodiscard]] float height() const noexcept {
            return height_;
        }

        [[nodiscard]] explicit operator bool() const noexcept {
            return handle_ != nullptr;
        }

        void reset() noexcept {
            handle_.reset();
            width_ = height_ = 0.0F;
        }

    private:
        TextureHandle handle_;
        float width_{ 0.0F };
        float height_{ 0.0F };

        void update_dimensions() {
            if (handle_) {
                SDL_GetTextureSize(handle_.get(), &width_, &height_);
            }
        }
    };

} // namespace sdl