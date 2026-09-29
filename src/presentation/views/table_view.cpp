#include "presentation/views/table_view.hpp"
#include "domain/dealer.hpp"
#include "domain/game.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace presentation {
    SDL_AppResult TableView::handle_event(const SDL_Event& event) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                return SDL_APP_SUCCESS;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        return SDL_APP_SUCCESS;
                    case SDLK_SPACE:
                        if (game_->can_hit()) {
                            game_->hit();
                        }
                        break;
                    case SDLK_RETURN:
                        if (game_->can_stand()) {
                            game_->stand();
                        }
                        break;
                    case SDLK_D:
                        if (game_->can_double_down()) {
                            game_->double_down();
                        }
                        break;
                    case SDLK_S:
                        if (game_->can_split()) {
                            game_->split();
                        }
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
        return SDL_APP_CONTINUE;
    };

    SDL_AppResult TableView::tick(float dt) {
        std::ignore = dt;        // TODO: unused for now
        std::ignore = textures_; // TODO: unused for now

        render_table_background();
        render_dealer_hand();
        render_player_hands();
        render_animations();
        switch (game_->state()) {
            case domain::GameState::WaitingForBets:
                render_betting_hud();
                break;
            case domain::GameState::DealerTurn:
            case domain::GameState::Dealing:
                break;
            case domain::GameState::PlayerTurn:
                render_action_buttons();
                break;
            case domain::GameState::RoundOver:
                render_round_summary_overlay();
                break;
            default:
                break;
        }

        return SDL_APP_CONTINUE;
    }

    void TableView::render_table_background() {
        SDL_SetRenderDrawColor(renderer_, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);

        // test: draw all 52 cards in a 13x4 grid to test the sprite sheet
        for (int suit_idx = 0; suit_idx < 4; ++suit_idx) {
            for (int rank_val = 2; rank_val <= 14; ++rank_val) {
                const domain::Card card(
                    static_cast<domain::Suit>(suit_idx),
                    static_cast<domain::Rank>(rank_val));

                const float x = 20.0F + (static_cast<float>(rank_val - 2) * 55.0F);
                const float y = 20.0F + (static_cast<float>(suit_idx) * 75.0F);

                card_sprites_.render(renderer_, card, SDL_FRect{ .x = x, .y = y, .w = 48.0F, .h = 64.0F });
            }
        }

        // Test card back
        card_sprites_.render_back(renderer_, SDL_FRect{ .x = 20.0F, .y = 330.0F, .w = 48.0F, .h = 64.0F });
        SDL_RenderPresent(renderer_);
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
