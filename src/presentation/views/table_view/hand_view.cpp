#include "presentation/views/table_view/hand_view.hpp"
#include <cstddef>

namespace presentation {

    HandView::HandView(SDL_Renderer* renderer, FontManager& fonts, CardSpriteSheet& card_sprites, ScorePlacement placement)
        : renderer_(renderer)
        , card_sprites_(&card_sprites)
        , score_label_(renderer_, fonts.lg(), "", color::Color{ 255, 255, 255, 255 })
        , placement_(placement) {
    }

    void HandView::render(const domain::Hand& hand, float start_x, float start_y, bool is_active) {
        const auto& cards = hand.cards();
        if (cards.empty()) {
            return;
        }

        const float active_step = card_sprites_->card_width() + 10.0F;
        const float inactive_step = card_sprites_->card_width() / 2.0F;
        const float card_step = is_active ? active_step : inactive_step;

        for (std::size_t i = 0; i < cards.size(); ++i) {
            const auto& dealt = cards.at(i);
            const SDL_FPoint position{
                .x = start_x + (static_cast<float>(i) * card_step),
                .y = start_y,
            };

            if (!dealt.is_face_up) {
                card_sprites_->render_back(renderer_, position);
            } else {
                card_sprites_->render(renderer_, dealt.card, position);
            }
        }

        if (score_label_.text().empty()) {
            return;
        }

        float score_y = 0.0F;
        if (placement_ == ScorePlacement::Below) {
            score_y = start_y + card_sprites_->card_height() + 10.0F;
        } else {
            score_y = start_y - score_label_.height() - 10.0F;
        }

        score_label_.render(start_x, score_y);
    }

    void HandView::set_score(std::string score_text) {
        score_label_.set_text(std::move(score_text));
    }

    void HandView::clear_score() {
        score_label_.set_text("");
    }

} // namespace presentation
