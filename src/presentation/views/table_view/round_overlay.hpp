#pragma once

#include "domain/game.hpp"
#include "presentation/assets/font_manager.hpp"
#include "presentation/components/text/text.hpp"
#include <SDL3/SDL_render.h>
#include <string_view>

namespace presentation {

    class RoundOverlay {
    public:
        RoundOverlay(SDL_Renderer* renderer, FontManager& fonts);

        void render();
        void show(const domain::RoundSummary& summary, int current_chips, std::string_view outcome);
        void clear();

    private:
        Text summary_label_;
        Text outcome_label_;
    };

} // namespace presentation
