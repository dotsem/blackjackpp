#pragma once

#include "domain/game.hpp"
#include "presentation/assets/card_sprite_sheet.hpp"
#include "presentation/assets/font_manager.hpp"
#include "presentation/assets/texture_manager.hpp"
#include "presentation/components/text/text.hpp"
#include "view.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace presentation {
    class TableView : public View {

    public:
        static constexpr float CARD_SCALE = 4.0F;

        TableView(SDL_Renderer* renderer, TextureManager& textures, FontManager& fonts, domain::Game* game)
            : renderer_(renderer)
            , textures_(&textures)
            , fonts_(&fonts)
            , game_(game)
            , card_sprites_(CardSpriteSheet::load(textures))
            , action_prompt_label_(renderer_, fonts_->xxxl(), "", SDL_Color{ .r = 255, .g = 255, .b = 255, .a = 255 })
            , player_score_label_(renderer_, fonts_->lg(), "", SDL_Color{ .r = 255, .g = 255, .b = 255, .a = 255 })
            , dealer_score_label_(renderer_, fonts_->lg(), "", SDL_Color{ .r = 255, .g = 255, .b = 255, .a = 255 }) {
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
        FontManager* fonts_;
        domain::Game* game_{ nullptr };
        CardSpriteSheet card_sprites_;

        Text action_prompt_label_;
        Text player_score_label_;
        Text dealer_score_label_;

        void render_table_background();
        void render_dealer_hand();
        void render_player_hands();
        void render_animations();
        void render_betting_hud();
        void render_action_buttons();
        void render_round_summary_overlay();
    };
}