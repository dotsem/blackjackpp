#pragma once

#include "domain/game.hpp"
#include "view.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace presentation {
    class TableView : public View {
    public:
        TableView() = default;
        ~TableView() override = default;

        TableView(const TableView&) = delete;
        TableView& operator=(const TableView&) = delete;

        TableView(TableView&&) noexcept = default;
        TableView& operator=(TableView&&) noexcept = default;

        SDL_AppResult tick(domain::Game& game) override;

        SDL_AppResult handle_event(const SDL_Event& event, domain::Game& game) override;

    private:
        void render_table_background();
        void render_dealer_hand(const domain::Dealer& dealer);
        void render_player_hands(const domain::Player& player);
        void render_animations();
        void render_betting_hud(const domain::Game& game);
        void render_action_buttons(domain::Game& game);
        void render_round_summary_overlay(const domain::Game& game);
    };
}