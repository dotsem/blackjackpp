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

        void render(SDL_Renderer* renderer, const domain::Card& card, SDL_FRect dst) const {
            const auto idx = card_index(card.suit(), card.rank(), config_.cols);
            const SDL_FRect& src = lut_.at(idx);
            SDL_RenderTexture(renderer, texture_, &src, &dst);
        }

        void render_back(SDL_Renderer* renderer, SDL_FRect dst) const {
            const SDL_FRect& src = lut_.at(CARD_BACK_IDX);
            SDL_RenderTexture(renderer, texture_, &src, &dst);
        }

    private:
        SDL_Texture* texture_{ nullptr };
        SheetConfig config_;
        std::array<SDL_FRect, 56> lut_{};

        static constexpr size_t card_index(domain::Suit suit, domain::Rank rank, int cols) noexcept {
            const auto r_idx = static_cast<size_t>(rank) - 2; // rank starts at 2
            const auto s_idx = static_cast<size_t>(suit);
            return (s_idx * cols) + r_idx;
        }

        /// Last row isn't really used as it contains jokers & the card backs
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