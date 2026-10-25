#include "presentation/views/table_view/betting_hud.hpp"
#include "presentation/color.hpp"
#include "presentation/layout.hpp"
#include <format>

namespace presentation {

    BettingHud::BettingHud(SDL_Renderer* renderer, FontManager& fonts)
        : balance_label_(renderer, fonts.lg(), "", color::Color{ 255, 255, 255, 255 })
        , action_prompt_label_(renderer, fonts.xxxl(), "", color::Color{ 255, 255, 255, 255 }) {
    }

    void BettingHud::render() {
        balance_label_.render(layout::TableCenterX, layout::BorderPadding, layout::Anchor::TopCenter);
        action_prompt_label_.render(layout::TableCenterX, layout::TableCenterY, layout::Anchor::Center);
    }

    void BettingHud::set_balance(int chips, int current_bet) {
        if (current_bet > 0) {
            balance_label_.set_text(std::format("Bet: {} | Balance: {}", current_bet, chips));
        } else {
            balance_label_.set_text(std::format("Balance: {}", chips));
        }
    }

    void BettingHud::set_prompt(std::string prompt) {
        action_prompt_label_.set_text(std::move(prompt));
    }

    void BettingHud::clear() {
        balance_label_.set_text("");
        action_prompt_label_.set_text("");
    }

} // namespace presentation
