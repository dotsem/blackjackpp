#include "presentation/views/table_view.hpp"
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

    SDL_AppResult TableView::tick() {
        return SDL_APP_CONTINUE;
    }
}