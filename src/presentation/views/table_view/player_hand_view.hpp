#pragma once

#include "SDL3/SDL_render.h"
#include "domain/player.hpp"
#include "presentation/assets/card_sprite_sheet.hpp"
#include "presentation/assets/font_manager.hpp"
#include "presentation/views/table_view/hand_view.hpp"
#include <vector>

namespace presentation {
    class PlayerHandView {
    public:
        PlayerHandView(SDL_Renderer* renderer, FontManager& fonts, CardSpriteSheet& card_sprites)
            : renderer_(renderer)
            , fonts_(&fonts)
            , card_sprites_(&card_sprites) {
            hand_views_.emplace_back(renderer, fonts, card_sprites, ScorePlacement::Above);
        }

        void add_hand() {
            hand_views_.emplace_back(renderer_, *fonts_, *card_sprites_, ScorePlacement::Above);
        }

        void reset() {
            hand_views_.clear();
            hand_views_.emplace_back(renderer_, *fonts_, *card_sprites_, ScorePlacement::Above);
        }

        void set_score(std::size_t hand_index, const std::string& score) {
            if (hand_index < hand_views_.size()) {
                hand_views_[hand_index].set_score(score);
            }
        }

        void clear_score(std::size_t hand_index) {
            if (hand_index < hand_views_.size()) {
                hand_views_[hand_index].clear_score();
            }
        }

        void clear_scores() {
            for (auto& hand_view : hand_views_) {
                hand_view.clear_score();
            }
        }

        void render(const std::vector<domain::PlayerHand>& hands, float start_x, float start_y, std::size_t active_hand_index) {
            const float hand_spacing = 300.0F; // TODO: dynamically calculate spacing based on number of hands and card width
            for (std::size_t i = 0; i < hands.size(); ++i) {
                const float hand_x = start_x + (static_cast<float>(i) * hand_spacing);
                hand_views_[i].render(hands.at(i).hand, hand_x, start_y, i == active_hand_index);
            }
        }

    private:
        SDL_Renderer* renderer_;
        FontManager* fonts_;
        CardSpriteSheet* card_sprites_;
        std::vector<HandView> hand_views_;
    };

}