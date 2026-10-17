#include "SDL3/SDL_stdinc.h"

namespace presentation {

    struct Transform2D {
        float x = 0.0F;
        float y = 0.0F;
        float scale = 1.0F;
        double angle = 0.0;
        Uint8 alpha = 255;
    };

    struct AnimationModifier {
        AnimationModifier() = default;
        virtual ~AnimationModifier() = default;
        AnimationModifier(const AnimationModifier&) = delete;
        AnimationModifier& operator=(const AnimationModifier&) = delete;
        AnimationModifier(AnimationModifier&&) = delete;
        AnimationModifier& operator=(AnimationModifier&&) = delete;
        virtual void update(Transform2D& t, float dt) = 0;
    };
}