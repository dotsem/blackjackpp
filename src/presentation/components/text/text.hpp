#pragma once

#include "SDL3/SDL_render.h"
#include "SDL_fur_coat/font.hpp"
#include "SDL_fur_coat/texture.hpp"
#include "presentation/color.hpp"
#include "presentation/components/animations/animations.hpp"
#include "presentation/layout.hpp"
#include <string>

namespace presentation {
    class Text {
    public:
        Text(SDL_Renderer* renderer, const sdl::Font& font, std::string text, color::Color color = { 255, 255, 255, 255 })
            : renderer_(renderer)
            , font_(&font)
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

        void render(float x, float y, layout::Anchor anchor = layout::Anchor::None) const {
            if (!texture_) {
                return;
            }

            const auto anchor_pos = layout::anchor(x, y, texture_.width(), texture_.height(), anchor);
            SDL_FRect dst{ .x = anchor_pos.x, .y = anchor_pos.y, .w = texture_.width(), .h = texture_.height() };
            SDL_RenderTexture(renderer_, texture_.get(), nullptr, &dst);
        }

        void render(float x, float y, const Transform2D& t, layout::Anchor anchor = layout::Anchor::None) const {
            if (!texture_) {
                return;
            }
            const float w = texture_.width() * t.scale;
            const float h = texture_.height() * t.scale;
            const auto pos = layout::anchor(x + t.x, y + t.y, w, h, anchor);
            const SDL_FRect dst{ .x = pos.x, .y = pos.y, .w = w, .h = h };
            SDL_SetTextureAlphaMod(texture_.get(), t.alpha);
            SDL_RenderTextureRotated(renderer_, texture_.get(), nullptr, &dst, t.angle, nullptr, SDL_FLIP_NONE);
        }

        void render(const SDL_FRect& rect, layout::Anchor anchor = layout::Anchor::Center) const {
            render(rect.x + (rect.w / 2.0F), rect.y + (rect.h / 2.0F), anchor);
        }

        void render(const SDL_FRect& rect, const Transform2D& t, layout::Anchor anchor = layout::Anchor::Center) const {
            render(rect.x + (rect.w / 2.0F), rect.y + (rect.h / 2.0F), t, anchor);
        }

        [[nodiscard]] const std::string& text() const noexcept {
            return text_;
        }

        [[nodiscard]] color::Color color() const noexcept {
            return color_;
        }

        void set_color(color::Color color) {
            if (color != color_) {
                color_ = color;
                rebuild_texture();
            }
        }

        [[nodiscard]] const sdl::Font* font() const noexcept {
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
        const sdl::Font* font_;
        color::Color color_;
        std::string text_;
        sdl::Texture texture_;

        void rebuild_texture();
    };
}