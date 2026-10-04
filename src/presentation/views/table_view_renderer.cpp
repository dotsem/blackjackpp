#include "SDL3/SDL_render.h"
#include "presentation/layout.hpp"
#include "table_view.hpp"
#include <format>
#include <string>

void centered_text(SDL_Renderer* renderer, const std::string& text, float scale) {
    constexpr float char_size = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
    const float text_w = static_cast<float>(text.length()) * char_size * scale;
    const float text_h = char_size * scale;
    const float draw_x = presentation::layout::centerX(text_w) / scale;
    const float draw_y = presentation::layout::centerY(text_h) / scale;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_SetRenderScale(renderer, scale, scale);
    SDL_RenderDebugText(renderer, draw_x, draw_y, text.c_str());
    SDL_SetRenderScale(renderer, 1.0F, 1.0F);
}

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
        // TODO: for now just focus on single hand, later add support for multiple hands
        const auto& player_hands = game_->player().hands();
        const auto active_hand_index = game_->player().active_hand_index();

        const float active_card_step = card_sprites_.card_width() + 10.0F;
        const float inactive_card_step = card_sprites_.card_width() / 2;

        for (size_t hand_idx = 0; hand_idx < player_hands.size(); ++hand_idx) {
            const auto& hand = player_hands.at(hand_idx).hand;
            for (size_t i = 0; i < hand.cards().size(); ++i) {
                const auto& dealt = hand.cards().at(i);

                const float card_step = active_hand_index == hand_idx ? active_card_step : inactive_card_step;
                const SDL_FPoint position{
                    // TODO: actually position split cards in brackets
                    .x = layout::BorderPadding + (static_cast<float>(i) * card_step),
                    .y = layout::VirtualHeight - layout::BorderPadding - card_sprites_.card_height()
                };

                card_sprites_.render(renderer_, dealt.card, position);
            }
        }
    }

    void TableView::render_animations() {
    }

    void TableView::render_betting_hud() {
        centered_text(renderer_, std::format("Chips: {}", game_->player().chips()), 4.0F);
    }

    void TableView::render_action_buttons() {
        centered_text(renderer_, "Hit or Stand", 4.0F);
    }

    void TableView::render_round_summary_overlay() {
        std::ignore = game_->state();
        centered_text(renderer_, std::format("{}", game_->player().active_hand().outcome_to_string()), 4.0F);
    }
}
