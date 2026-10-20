#pragma once

#include "domain/game.hpp"
#include "presentation/assets/card_sprite_sheet.hpp"
#include "presentation/assets/font_manager.hpp"
#include "presentation/assets/texture_manager.hpp"
#include "presentation/views/table_view/betting_hud.hpp"
#include "presentation/views/table_view/hand_view.hpp"
#include "presentation/views/table_view/player_hand_view.hpp"
#include "presentation/views/table_view/round_overlay.hpp"
#include "presentation/views/table_view/table_background.hpp"
#include "presentation/views/view.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace presentation {

    class TableView : public View {
    public:
        static constexpr float CARD_SCALE = 4.0F;
        static constexpr int BET_STEP = 25;
        static constexpr int MIN_BET = 50;

        TableView(SDL_Renderer* renderer, TextureManager& textures, FontManager& fonts, domain::Game* game);
        ~TableView() override = default;

        TableView(const TableView&) = delete;
        TableView& operator=(const TableView&) = delete;

        TableView(TableView&&) noexcept = default;
        TableView& operator=(TableView&&) noexcept = default;

        SDL_AppResult tick(float dt) override;
        SDL_AppResult handle_event(const SDL_Event& event) override;

    private:
        SDL_Renderer* renderer_;
        // TextureManager* textures_;
        // FontManager* fonts_;
        domain::Game* game_{ nullptr };
        CardSpriteSheet card_sprites_;

        TableBackground background_;
        HandView dealer_hand_view_;
        PlayerHandView player_hand_view_;
        BettingHud betting_hud_;
        RoundOverlay round_overlay_;

        int temp_bet_amount_{ MIN_BET };

        void poll_game_events();
        void apply_event(const domain::GameEvent& event);
        void on_state_changed(const domain::GameStateChangedEvent& event);
        void on_round_over(const domain::RoundOverEvent& event);

        SDL_AppResult handle_key_down(SDL_Keycode key);
        void adjust_bet(int delta);
    };

} // namespace presentation