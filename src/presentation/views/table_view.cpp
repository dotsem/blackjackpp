#include "presentation/views/table_view.hpp"
#include "domain/game.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <iostream>

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
                            // TODO: disabled until UI is ready
                            // game_->split();
                        }
                    case SDLK_B:
                        if (game_->state() == domain::GameState::WaitingForBets) {
                            game_->place_bet(10);
                            game_->set_state(domain::GameState::Dealing);
                            game_->deal_initial_cards();
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
        SDL_RenderPresent(renderer_);

        return SDL_APP_CONTINUE;
    }

}
