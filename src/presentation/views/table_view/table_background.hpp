#pragma once

#include "presentation/assets/card_sprite_sheet.hpp"
#include <SDL3/SDL_render.h>
#include <cstddef>

namespace presentation {

    class TableBackground {
    public:
        TableBackground(SDL_Renderer* renderer, CardSpriteSheet& card_sprites);

        void render(std::size_t remaining_deck_cards);

    private:
        SDL_Renderer* renderer_;
        CardSpriteSheet* card_sprites_;
    };

} // namespace presentation
