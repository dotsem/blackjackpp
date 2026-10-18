#include "presentation/views/table_view/table_background.hpp"
#include "presentation/layout.hpp"
#include <algorithm>

namespace presentation {

    TableBackground::TableBackground(SDL_Renderer* renderer, CardSpriteSheet& card_sprites)
        : renderer_(renderer)
        , card_sprites_(&card_sprites) {
    }

    void TableBackground::render(std::size_t remaining_deck_cards) {
        // Table felt: #0D4D24
        SDL_SetRenderDrawColor(renderer_, 13, 77, 36, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);

        const float card_step = layout::VirtualWidth - layout::BorderPadding - card_sprites_->card_width();

        for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(5), remaining_deck_cards); ++i) {
            card_sprites_->render_back(
                renderer_,
                SDL_FPoint{
                    .x = card_step - (static_cast<float>(i) * 2.0F * card_sprites_->scale()),
                    .y = layout::BorderPadding });
        }
    }

} // namespace presentation
