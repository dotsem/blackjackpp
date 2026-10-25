#pragma once

#include "SDL3/SDL_pixels.h"
#include <algorithm>

namespace presentation::color {

    struct Color : public SDL_Color {
        constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) noexcept
            : SDL_Color{ .r = r, .g = g, .b = b, .a = a } {
        }

        friend constexpr bool operator==(const SDL_Color& lhs, const SDL_Color& rhs) noexcept {
            return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
        }

        [[nodiscard]] constexpr float luminance() const noexcept {
            return ((0.2126F * static_cast<float>(r)) +
                       (0.7152F * static_cast<float>(g)) +
                       (0.0722F * static_cast<float>(b))) /
                   255.0F;
        }

        [[nodiscard]] constexpr Color contrast_color() const noexcept {
            return luminance() > 0.5F ? Color{ 0, 0, 0, 255 } : Color{ 255, 255, 255, 255 };
        }

        [[nodiscard]] constexpr Color with_alpha(Uint8 new_alpha) const noexcept {
            return Color{ r, g, b, new_alpha };
        }

        [[nodiscard]] constexpr Color darken(float factor = 0.2F) const noexcept {
            const float mult = std::clamp(1.0F - factor, 0.0F, 1.0F);
            return Color{
                static_cast<Uint8>(static_cast<float>(r) * mult),
                static_cast<Uint8>(static_cast<float>(g) * mult),
                static_cast<Uint8>(static_cast<float>(b) * mult),
                a
            };
        }
    };

    constexpr Color White{ 255, 255, 255, 255 };
    constexpr Color Black{ 0, 0, 0, 255 };
    constexpr Color Red{ 255, 0, 0, 255 };
    constexpr Color Green{ 0, 255, 0, 255 };
    constexpr Color Blue{ 0, 0, 255, 255 };
    constexpr Color Yellow{ 255, 255, 0, 255 };
    constexpr Color Cyan{ 0, 255, 255, 255 };
    constexpr Color Magenta{ 255, 0, 255, 255 };
    constexpr Color Gray{ 128, 128, 128, 255 };
    constexpr Color DarkGray{ 64, 64, 64, 255 };
    constexpr Color LightGray{ 192, 192, 192, 255 };
    constexpr Color Orange{ 255, 165, 0, 255 };
    constexpr Color Purple{ 128, 0, 128, 255 };
    constexpr Color Brown{ 165, 42, 42, 255 };
    constexpr Color Transparent{ 0, 0, 0, 0 };
}