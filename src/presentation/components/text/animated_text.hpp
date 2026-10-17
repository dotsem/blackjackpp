#pragma once

#include "presentation/components/animations/animations.hpp"
#include "text.hpp"
#include <vector>

namespace presentation {
    class AnimatedText {
    public:
        explicit AnimatedText(Text text)
            : text_(std::move(text)) {
        }

        AnimatedText& pulse(float speed = 5.0F, float amp = 0.15F) {
            modifiers_.push_back(std::make_unique<PulseModifier>(speed, amp));
            return *this;
        }

        AnimatedText& rotate(double deg_per_sec = 45.0) {
            modifiers_.push_back(std::make_unique<RotateModifier>(deg_per_sec));
            return *this;
        }

        void tick(float dt) {
            current_transform_ = Transform2D{};
            for (auto& mod : modifiers_) {
                mod->update(current_transform_, dt);
            }
        }

        void render(float x, float y, layout::Anchor anchor = layout::Anchor::None) const {
            text_.render(
                x + current_transform_.x,
                y + current_transform_.y,
                Transform2D{
                    .scale = current_transform_.scale,
                    .angle = current_transform_.angle,
                    .alpha = current_transform_.alpha },
                anchor);
        }

        void render(const SDL_FRect& rect, layout::Anchor anchor = layout::Anchor::Center) const {
            text_.render(
                rect.x + (rect.w / 2.0F),
                rect.y + (rect.h / 2.0F),
                Transform2D{
                    .scale = current_transform_.scale,
                    .angle = current_transform_.angle,
                    .alpha = current_transform_.alpha },
                anchor);
        }

    private:
        Text text_;
        Transform2D current_transform_{};
        std::vector<std::unique_ptr<AnimationModifier>> modifiers_;
    };
}