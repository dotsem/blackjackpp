#include "presentation/layout.hpp"
#include "table_view.hpp"

namespace presentation {
    void TableView::render_table_background() {
        // #0D4D24
        SDL_SetRenderDrawColor(renderer_, 13, 77, 36, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);
        // TODO: add vignette effect and table felt texture

        const float card_step = layout::VirtualWidth - layout::BorderPadding - card_sprites_.card_width();

        for (size_t i = 0; i < std::min(static_cast<size_t>(5), game_->deck().cards().size()); ++i) {
            card_sprites_.render_back(renderer_, SDL_FPoint{ .x = card_step - (static_cast<float>(i) * 2.0F * card_sprites_.scale()), .y = layout::BorderPadding });
        }
    }

    void TableView::render_dealer_hand() {
        const auto& dealer_cards = game_->dealer().hand().cards();
        const float card_step = card_sprites_.card_width() + 10.0F;

        for (size_t i = 0; i < dealer_cards.size(); ++i) {
            const auto& dealt = dealer_cards.at(i);

            const SDL_FPoint position{
                .x = layout::BorderPadding + (static_cast<float>(i) * card_step),
                .y = layout::BorderPadding
            };

            if (!dealt.is_face_up) {
                card_sprites_.render_back(renderer_, position);
            } else {
                card_sprites_.render(renderer_, dealt.card, position);
            }
        }
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