#pragma once

#include "animation_modifier.hpp"
#include <cmath>

namespace presentation {
    class PulseModifier : public AnimationModifier {
        float speed_, amplitude_, timer_{ 0.0F };

    public:
        PulseModifier(float speed = 5.0F, float amp = 0.15F)
            : speed_(speed)
            , amplitude_(amp) {
        }

        void update(Transform2D& t, float dt) override {
            timer_ += dt;
            t.scale *= (1.0F + (amplitude_ * std::sin(timer_ * speed_)));
        }
    };

    class RotateModifier : public AnimationModifier {
        double speed_deg_per_sec_;

    public:
        explicit RotateModifier(double deg_per_sec = 45.0)
            : speed_deg_per_sec_(deg_per_sec) {
        }

        void update(Transform2D& t, float dt) override {
            t.angle += speed_deg_per_sec_ * dt;
        }
    };
}