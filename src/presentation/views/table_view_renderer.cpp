#include "presentation/layout.hpp"
#include "table_view.hpp"

namespace presentation {
    void TableView::render_table_background() {
        // #0D4D24
        SDL_SetRenderDrawColor(renderer_, 13, 77, 36, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);
        // TODO: add vignette effect and table felt texture

        for (size_t i = 0; i < std::min(static_cast<size_t>(5), game_->deck().cards().size()); ++i) {
            card_sprites_.render_back(renderer_, SDL_FPoint{ .x = layout::VirtualWidth - card_sprites_.card_width() - (static_cast<float>(i) * 2.0F * card_sprites_.scale()), .y = 120.0F });
        }
    }

    void TableView::render_dealer_hand() {
        std::ignore = game_->dealer().hand();
    }

    void TableView::render_player_hands() {
        std::ignore = game_->player().hands();
    }

    void TableView::render_animations() {
    }

    void TableView::render_betting_hud() {
        std::ignore = game_->state();
    }

    void TableView::render_action_buttons() {
        std::ignore = game_->state();
    }

    void TableView::render_round_summary_overlay() {
        std::ignore = game_->state();
    }
}