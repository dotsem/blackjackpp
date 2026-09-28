#include "presentation/views/table_view.hpp"
#include "domain/dealer.hpp"
#include "domain/game.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace presentation {
    SDL_AppResult TableView::handle_event(const SDL_Event& event, domain::Game& game) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                return SDL_APP_SUCCESS;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        return SDL_APP_SUCCESS;
                    case SDLK_SPACE:
                        if (game.can_hit()) {
                            game.hit();
                        }
                        break;
                    case SDLK_RETURN:
                        if (game.can_stand()) {
                            game.stand();
                        }
                        break;
                    case SDLK_D:
                        if (game.can_double_down()) {
                            game.double_down();
                        }
                        break;
                    case SDLK_S:
                        if (game.can_split()) {
                            game.split();
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

    SDL_AppResult TableView::tick(domain::Game& game) {
        render_table_background();
        render_dealer_hand(game.dealer());
        render_player_hands(game.player());
        render_animations();
        switch (game.state()) {
            case domain::GameState::WaitingForBets:
                render_betting_hud(game);
                break;
            case domain::GameState::DealerTurn:
            case domain::GameState::Dealing:
                break;
            case domain::GameState::PlayerTurn:
                render_action_buttons(game);
                break;
            case domain::GameState::RoundOver:
                render_round_summary_overlay(game);
                break;
            default:
                break;
        }

        return SDL_APP_CONTINUE;
    }

    void render_table_background() {
    }

    void render_dealer_hand(domain::Dealer& dealer) {
        std::ignore = dealer.hand();
    }

    void render_player_hands(domain::Player& player) {
        std::ignore = player.hands();
    }

    void render_animations() {
    }

    void render_betting_hub(domain::Game& game) {
        std::ignore = game.state();
    }

    void render_action_buttons(domain::Game& game) {
        std::ignore = game.state();
    }

    void render_round_summary_overlay(domain::Game& game) {
        std::ignore = game.state();
    }

}
