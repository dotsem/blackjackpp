#pragma once

#include "presentation/assets/font_manager.hpp"
#include "presentation/components/text/text.hpp"
#include <SDL3/SDL_render.h>
#include <string>

namespace presentation {

    class BettingHud {
    public:
        BettingHud(SDL_Renderer* renderer, FontManager& fonts);

        void render();
        void set_balance(int chips, int current_bet = 0);
        void set_prompt(std::string prompt);
        void clear();

    private:
        Text balance_label_;
        Text action_prompt_label_;
    };

} // namespace presentation
