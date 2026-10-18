#include "presentation/views/table_view/round_overlay.hpp"
#include "presentation/layout.hpp"
#include <format>

namespace presentation {

    RoundOverlay::RoundOverlay(SDL_Renderer* renderer, FontManager& fonts)
        : summary_label_(renderer, fonts.lg(), "", SDL_Color{ .r = 255, .g = 255, .b = 255, .a = 255 })
        , outcome_label_(renderer, fonts.xxxl(), "", SDL_Color{ .r = 255, .g = 255, .b = 255, .a = 255 }) {
    }

    void RoundOverlay::render() {
        summary_label_.render(layout::TableCenterX, layout::BorderPadding, layout::Anchor::TopCenter);
        outcome_label_.render(layout::TableCenterX, layout::TableCenterY, layout::Anchor::Center);
    }

    void RoundOverlay::show(const domain::RoundSummary& summary, int current_chips, std::string_view outcome) {
        if (summary.is_profit()) {
            summary_label_.set_text(std::format("Win: {} | Balance: {}", summary.net_profit(), current_chips));
        } else {
            summary_label_.set_text(std::format("Loss: {} | Balance: {}", summary.net_profit(), current_chips));
        }

        outcome_label_.set_text(std::string(outcome));
    }

    void RoundOverlay::clear() {
        summary_label_.set_text("");
        outcome_label_.set_text("");
    }

} // namespace presentation
