#include "presentation/views/table_view/table_view.hpp"
#include "SDL3/SDL_keycode.h"
#include "domain/event.hpp"
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
        , player_hand_view_(renderer_, fonts, card_sprites_)
        , betting_hud_(renderer_, fonts)
        , round_overlay_(renderer_, fonts) {
        card_sprites_.set_scale(CARD_SCALE);
        betting_hud_.set_balance(game_->player().chips(), game_->total_bet());
        betting_hud_.set_prompt(std::format("Play Blackjack! (B: Bet {})", temp_bet_amount_));
        poll_game_events();
    }

    SDL_AppResult TableView::handle_event(const SDL_Event& event) {
        if (event.type == SDL_EVENT_QUIT) {
            return SDL_APP_SUCCESS;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            return handle_key_down(event.key.key);
        }
        return SDL_APP_CONTINUE;
    }

    SDL_AppResult TableView::handle_key_down(SDL_Keycode key) {
        switch (key) {
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
                } else if (game_->state() == domain::GameState::RoundOver) {
                    game_->next_round();
                }
                break;
            case SDLK_D:
                if (game_->can_double_down()) {
                    game_->double_down();
                }
                break;
            case SDLK_S:
                if (game_->can_split()) {
                    // game_->split();
                }
                break;
            case SDLK_B:
                if (game_->state() == domain::GameState::WaitingForBets && game_->can_place_bet(temp_bet_amount_)) {
                    game_->start_round(temp_bet_amount_);
                }
                break;
            case SDLK_UP:
                adjust_bet(BET_STEP);
                break;
            case SDLK_DOWN:
                adjust_bet(-BET_STEP);
                break;
            default:
                break;
        }
        return SDL_APP_CONTINUE;
    }

    void TableView::adjust_bet(int delta) {
        const int new_bet = temp_bet_amount_ + delta;
        if (new_bet >= MIN_BET && game_->can_place_bet(new_bet)) {
            temp_bet_amount_ = new_bet;
            betting_hud_.set_prompt(std::format("Play Blackjack! (B: Bet {})", temp_bet_amount_));
        }
    }

    SDL_AppResult TableView::tick(float dt) {
        std::ignore = dt;

        poll_game_events();

        background_.render(game_->deck().cards().size());

        dealer_hand_view_.render(game_->dealer().hand(), layout::BorderPadding, layout::BorderPadding);

        const auto& player_hands = game_->player().hands();
        const auto active_idx = game_->player().active_hand_index();
        const float player_y = layout::VirtualHeight - layout::BorderPadding - card_sprites_.card_height();

        player_hand_view_.render(player_hands, layout::BorderPadding, player_y, active_idx);

        if (game_->state() == domain::GameState::RoundOver) {
            round_overlay_.render();
        } else {
            betting_hud_.render();
        }

        SDL_RenderPresent(renderer_);
        return SDL_APP_CONTINUE;
    }

    /// Polls the game for events and updates the view accordingly.
    /// events not listed in here:
    /// - DeckReshuffledEvent: TODO: handle shoe reshuffle notification
    /// - HandStoodEvent
    /// - GameStateChangedEvent: Present but empty to satisfy clang-tidy
    ///     - Dealing
    //      - RoundOver
    void TableView::poll_game_events() {
        for (const auto& event : game_->poll_events()) {
            apply_event(event);
        }
    }

    void TableView::on_state_changed(const domain::GameStateChangedEvent& event) {
        switch (event.new_state) {
            case domain::GameState::WaitingForBets:
                betting_hud_.set_balance(game_->player().chips(), game_->total_bet());
                if (game_->player().chips() < MIN_BET) {
                    betting_hud_.set_prompt("You lost all your money! Restart your computer to play again.");
                } else {
                    betting_hud_.set_prompt(std::format("Play Blackjack! (B: Bet {})", temp_bet_amount_));
                }
                dealer_hand_view_.clear_score();
                player_hand_view_.clear_scores();
                round_overlay_.clear();
                break;
            case domain::GameState::Dealing:
            case domain::GameState::RoundOver:
                break;
            case domain::GameState::PlayerTurn:
                betting_hud_.set_prompt("Hit (SPACE) Or Stand (ENTER)");
                break;
            case domain::GameState::DealerTurn:
                betting_hud_.set_prompt("Dealer's Turn...");
                break;
        }
    }

    void TableView::on_round_over(const domain::RoundOverEvent& event) {
        temp_bet_amount_ = MIN_BET;
        betting_hud_.set_balance(event.final_chips, 0);
        round_overlay_.show(
            game_->round_summary(),
            event.final_chips,
            game_->player().active_hand().outcome_to_string());
        player_hand_view_.set_score(game_->player().active_hand_index(),
            std::format("{}", game_->player().active_hand().hand.value()));
        dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value()));
    }

    void TableView::apply_event(const domain::GameEvent& event) {
        if (const auto* e = std::get_if<domain::GameStateChangedEvent>(&event)) {
            on_state_changed(*e);
        } else if (std::get_if<domain::DeckReshuffledEvent>(&event) != nullptr) {
            // TODO: handle shoe reshuffle notification
        } else if (const auto* e = std::get_if<domain::BetPlacedEvent>(&event)) {
            betting_hud_.set_balance(e->remaining_chips, e->total_bet);
        } else if (std::get_if<domain::RoundStartedEvent>(&event) != nullptr) {
            dealer_hand_view_.clear_score();
            player_hand_view_.clear_scores();
            round_overlay_.clear();
        } else if (const auto* e = std::get_if<domain::CardDealtEvent>(&event)) {
            if (e->is_dealer) {
                dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value(true)));
            } else {
                player_hand_view_.set_score(game_->player().active_hand_index(),
                    std::format("{}", game_->player().active_hand().hand.value()));
            }
        } else if (std::get_if<domain::ActiveHandChangedEvent>(&event) != nullptr ||
                   std::get_if<domain::HandBustedEvent>(&event) != nullptr) {
            player_hand_view_.set_score(game_->player().active_hand_index(),
                std::format("{}", game_->player().active_hand().hand.value()));
        } else if (const auto* e = std::get_if<domain::HandSplitEvent>(&event)) {
            betting_hud_.set_balance(e->remaining_chips, game_->total_bet());
            player_hand_view_.set_score(game_->player().active_hand_index(),
                std::format("{}", game_->player().active_hand().hand.value()));
        } else if (const auto* e = std::get_if<domain::HandDoubledDownEvent>(&event)) {
            betting_hud_.set_balance(e->remaining_chips, game_->total_bet());
            player_hand_view_.set_score(game_->player().active_hand_index(),
                std::format("{}", game_->player().active_hand().hand.value()));
        } else if (std::get_if<domain::DealerTurnStartedEvent>(&event) != nullptr) {
            betting_hud_.set_prompt("Dealer's Turn...");
        } else if (std::get_if<domain::HoleCardRevealedEvent>(&event) != nullptr) {
            dealer_hand_view_.set_score(std::format("{}", game_->dealer().hand().value(true)));
        } else if (const auto* e = std::get_if<domain::RoundOverEvent>(&event)) {
            on_round_over(*e);
        }
    }

} // namespace presentation
