#pragma once

#include "SDL3/SDL_render.h"
#include "SDL_fur_coat/font.hpp"
#include "SDL_fur_coat/texture.hpp"
#include <string>

namespace presentation {
    class Text {
    public:
        Text(SDL_Renderer* renderer, sdl::Font* font, std::string text, SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 })
            : renderer_(renderer)
            , font_(font)
            , color_(color) {
            set_text(std::move(text));
        }

        void set_text(std::string text) {
            if (text == text_ && texture_) {
                return;
            }

            text_ = std::move(text);
            rebuild_texture();
        }

        void render(float x, float y) {
            if (!texture_) {
                return;
            }

            SDL_FRect dst{ .x = x, .y = y, .w = texture_.width(), .h = texture_.height() };
            SDL_RenderTexture(renderer_, texture_.get(), nullptr, &dst);
        }

        void render_centered(float center_x, float center_y) {
            render(center_x - (texture_.width() / 2.0F), center_y - (texture_.height() / 2.0F));
        }

        void render_centered_horizontally(float center_x, float y) {
            render(center_x - (texture_.width() / 2.0F), y);
        }

        void render_centered_vertically(float x, float center_y) {
            render(x, center_y - (texture_.height() / 2.0F));
        }

        void render_centered_in_rect(const SDL_FRect& rect) {
            render(rect.x + (rect.w / 2.0F) - (texture_.width() / 2.0F), rect.y + (rect.h / 2.0F) - (texture_.height() / 2.0F));
        }

        [[nodiscard]] const std::string& text() const noexcept {
            return text_;
        }

        [[nodiscard]] SDL_Color color() const noexcept {
            return color_;
        }

        [[nodiscard]] sdl::Font* font() const noexcept {
            return font_;
        }

        [[nodiscard]] SDL_Renderer* renderer() const noexcept {
            return renderer_;
        }

        [[nodiscard]] bool is_empty() const noexcept {
            return text_.empty();
        }

        [[nodiscard]] bool has_texture() const noexcept {
            return static_cast<bool>(texture_);
        }

        [[nodiscard]] sdl::Texture& texture() noexcept {
            return texture_;
        }

        [[nodiscard]] const sdl::Texture& texture() const noexcept {
            return texture_;
        }

        [[nodiscard]] SDL_Texture* raw_texture() const noexcept {
            return texture_.get();
        }

        [[nodiscard]] bool is_valid() const noexcept {
            return font_ != nullptr && renderer_ != nullptr;
        }

        [[nodiscard]] bool is_renderable() const noexcept {
            return is_valid() && !text_.empty();
        }

        [[nodiscard]] bool is_renderable_with_texture() const noexcept {
            return is_renderable() && has_texture();
        }

        [[nodiscard]] float width() const noexcept {
            return texture_.width();
        }

        [[nodiscard]] float height() const noexcept {
            return texture_.height();
        }

    private:
        SDL_Renderer* renderer_;
        sdl::Font* font_;
        SDL_Color color_;
        std::string text_;
        sdl::Texture texture_;

        void rebuild_texture();
    };
}