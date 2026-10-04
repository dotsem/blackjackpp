#pragma once

#include "domain/card.hpp"
#include "presentation/assets/texture_manager.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <array>

namespace {
    constexpr int CARD_BACK_IDX = 27;
}

namespace presentation {

    class CardSpriteSheet {
    public:
        struct SheetConfig {
            float margin_x{ 11.0F };
            float margin_y{ 2.0F };
            float gap_x{ 23.0F };
            float gap_y{ 5.0F };
            float card_w{ 42.0F };
            float card_h{ 60.0F };
            int cols{ 14 };
        };

        static CardSpriteSheet load(TextureManager& textures) {
            auto* texture = textures.load("assets/cards.png");
            if (texture == nullptr) {
                throw std::runtime_error("Failed to load card sprite sheet texture");
            }
            return CardSpriteSheet(texture, SheetConfig{});
        }

        CardSpriteSheet(SDL_Texture* texture, const SheetConfig& cfg)
            : texture_(texture)
            , config_(cfg) {
            build_lookup_table();
        }

        void render(SDL_Renderer* renderer, const domain::Card& card, SDL_FPoint dst) const {
            const auto idx = card_index(card.suit(), card.rank(), config_.cols);
            render_(renderer, idx, dst);
        }

        void render_back(SDL_Renderer* renderer, SDL_FPoint dst) const {
            render_(renderer, CARD_BACK_IDX, dst);
        }

        [[nodiscard]] float card_width() const noexcept {
            return config_.card_w * scale_;
        }

        [[nodiscard]] float card_height() const noexcept {
            return config_.card_h * scale_;
        }

        [[nodiscard]] float scale() const noexcept {
            return scale_;
        }

        void set_scale(float scale) noexcept {
            scale_ = scale;
        }

    private:
        SDL_Texture* texture_{ nullptr };
        SheetConfig config_;
        float scale_{ 1.0F };
        std::array<SDL_FRect, 56> lut_{};

        void render_(SDL_Renderer* renderer, int idx, SDL_FPoint position) const {
            const SDL_FRect dst{
                .x = position.x,
                .y = position.y,
                .w = card_width(),
                .h = card_height()
            };
            const SDL_FRect& src = lut_.at(idx);
            SDL_RenderTexture(renderer, texture_, &src, &dst);
        }

        static constexpr int card_index(domain::Suit suit, domain::Rank rank, int cols) noexcept {
            const auto r_idx = static_cast<int>(rank) - 1; // ace rank starts at 1
            const auto s_idx = static_cast<int>(suit);
            return (s_idx * cols) + r_idx;
        }

        /// Last column isn't really used as it contains jokers & the card backs
        /// It is just there to render the card back
        void build_lookup_table() noexcept {
            const auto cols = static_cast<size_t>(config_.cols);
            for (size_t i = 0; i < 56; ++i) {
                const auto col_idx = i % cols;
                const auto row_idx = i / cols;

                lut_.at(i) = SDL_FRect{
                    .x = config_.margin_x + (static_cast<float>(col_idx) * (config_.card_w + config_.gap_x)),
                    .y = config_.margin_y + (static_cast<float>(row_idx) * (config_.card_h + config_.gap_y)),
                    .w = config_.card_w,
                    .h = config_.card_h
                };
            }
        }
    };

} // namespace presentation