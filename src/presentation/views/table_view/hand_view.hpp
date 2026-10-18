#pragma once

#include "domain/hand.hpp"
#include "presentation/assets/card_sprite_sheet.hpp"
#include "presentation/assets/font_manager.hpp"
#include "presentation/components/text/text.hpp"
#include <SDL3/SDL_render.h>
#include <string>

namespace presentation {

    enum class ScorePlacement : std::uint8_t {
        Above,
        Below
    };

    class HandView {
    public:
        HandView(SDL_Renderer* renderer, FontManager& fonts, CardSpriteSheet& card_sprites, ScorePlacement placement);

        void render(const domain::Hand& hand, float start_x, float start_y, bool is_active = true);
        void set_score(std::string score_text);
        void clear();

    private:
        SDL_Renderer* renderer_;
        CardSpriteSheet* card_sprites_;
        Text score_label_;
        ScorePlacement placement_;
    };

} // namespace presentation
