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

        SDL_AppResult tick() override;

        SDL_AppResult handle_event(const SDL_Event& event, domain::Game& game) override;
    };
}