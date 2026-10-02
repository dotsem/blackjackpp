#pragma once

#include "domain/game.hpp"
#include "presentation/assets/card_sprite_sheet.hpp"
#include "presentation/assets/texture_manager.hpp"
#include "view.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace presentation {
    class TableView : public View {

    public:
        static constexpr float CARD_SCALE = 8.0F;

        TableView(SDL_Renderer* renderer, TextureManager& textures, domain::Game* game)
            : renderer_(renderer)
            , textures_(&textures)
            , game_(game)
            , card_sprites_(CardSpriteSheet::load(textures)) {
            card_sprites_.set_scale(CARD_SCALE);
        }

        ~TableView() override = default;

        TableView(const TableView&) = delete;
        TableView& operator=(const TableView&) = delete;

        TableView(TableView&&) noexcept = default;
        TableView& operator=(TableView&&) noexcept = default;

        SDL_AppResult tick(float dt) override;

        SDL_AppResult handle_event(const SDL_Event& event) override;

    private:
        SDL_Renderer* renderer_;
        TextureManager* textures_;
        domain::Game* game_{ nullptr };
        CardSpriteSheet card_sprites_;

        void render_table_background();
        void render_dealer_hand();
        void render_player_hands();
        void render_animations();
        void render_betting_hud();
        void render_action_buttons();
        void render_round_summary_overlay();
    };
}