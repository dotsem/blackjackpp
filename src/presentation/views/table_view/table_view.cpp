#include "presentation/views/table_view/table_view.hpp"
#include "domain/game.hpp"
#include "presentation/layout.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <format>

namespace presentation {

    TableView::TableView(SDL_Renderer* renderer, TextureManager& textures, FontManager& fonts, domain::Game* game)
        : renderer_(renderer)
        // , textures_(&textures)
        // , fonts_(&fonts)
        , game_(game)
        , card_sprites_(CardSpriteSheet::load(textures))
        , background_(renderer_, card_sprites_)
        , dealer_hand_view_(renderer_, fonts, card_sprites_, ScorePlacement::Below)
        , player_hand_view_(renderer_, fonts, card_sprites_, ScorePlacement::Above)
        , betting_hud_(renderer_, fonts)
        , round_overlay_(renderer_, fonts) {
        card_sprites_.set_scale(CARD_SCALE);
        sync_ui();
    }

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
                            sync_ui();
                        }
                        break;
                    case SDLK_RETURN:
                        if (game_->can_stand()) {
                            game_->stand();
                        } else if (game_->state() == domain::GameState::RoundOver) {
                            game_->next_round();
                        }
                        sync_ui();
                        break;
                    case SDLK_D:
                        if (game_->can_double_down()) {
                            game_->double_down();
                            sync_ui();
                        }
                        break;
                    case SDLK_S:
                        if (game_->can_split()) {
                            // game_->split();
                            sync_ui();
                        }
                        break;
                    case SDLK_B:
                        if (game_->state() == domain::GameState::WaitingForBets) {
                            if (game_->can_place_bet(10)) {
                                game_->start_round(10);
                                sync_ui();
                            }
                        }
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
        return SDL_APP_CONTINUE;
    }

    SDL_AppResult TableView::tick(float dt) {
        std::ignore = dt;

        background_.render(game_->deck().cards().size());

        dealer_hand_view_.render(game_->dealer().hand(), layout::BorderPadding, layout::BorderPadding);

        const auto& player_hands = game_->player().hands();
        const auto active_idx = game_->player().active_hand_index();
        const float player_y = layout::VirtualHeight - layout::BorderPadding - card_sprites_.card_height();
        const float hand_spacing = 300.0F; // TODO: dynamically calculate spacing based on number of hands and card width

        for (std::size_t i = 0; i < player_hands.size(); ++i) {
            const float hand_x = layout::BorderPadding + (static_cast<float>(i) * hand_spacing);
            player_hand_view_.render(player_hands.at(i).hand, hand_x, player_y, i == active_idx);
        }

        if (game_->state() == domain::GameState::RoundOver) {
            round_overlay_.render();
        } else {
            betting_hud_.render();
        }

        SDL_RenderPresent(renderer_);
        return SDL_APP_CONTINUE;
    }

    void TableView::sync_ui() {
        switch (game_->state()) {
            case domain::GameState::WaitingForBets:
                betting_hud_.set_balance(game_->player().chips(), game_->total_bet());
                betting_hud_.set_prompt("Play Blackjack! (B: Bet 10)");
                dealer_hand_view_.clear();
                player_hand_view_.clear();
                round_overlay_.clear();
                break;
            case domain::GameState::Dealing:
                betting_hud_.set_balance(game_->player().chips(), game_->total_bet());
                player_hand_view_.set_score(std::format("{}", game_->player().active_hand().hand.value()));
                dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value(true)));
                break;
            case domain::GameState::PlayerTurn:
                betting_hud_.set_balance(game_->player().chips(), game_->total_bet());
                betting_hud_.set_prompt("Hit (SPACE) Or Stand (ENTER)");
                player_hand_view_.set_score(std::format("{}", game_->player().active_hand().hand.value()));
                dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value(true)));
                break;
            case domain::GameState::DealerTurn:
                betting_hud_.set_balance(game_->player().chips(), game_->total_bet());
                dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value(true)));
                break;
            case domain::GameState::RoundOver:
                round_overlay_.show(
                    game_->round_summary(),
                    game_->player().chips(),
                    game_->player().active_hand().outcome_to_string());
                player_hand_view_.set_score(std::format("{}", game_->player().active_hand().hand.value()));
                dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value()));
                break;
        }
    }

} // namespace presentation
