#pragma once

#include "SDL3/SDL_rect.h"
#include <cstdint>

namespace presentation::layout {
    inline constexpr float VirtualWidth = 1920.0F;
    inline constexpr float VirtualHeight = 1080.0F;
    inline constexpr float AspectRatio = VirtualWidth / VirtualHeight;
    inline constexpr float TableCenterX = VirtualWidth / 2.0F;
    inline constexpr float TableCenterY = VirtualHeight / 2.0F;
    inline constexpr float BorderPadding = 20.0F;

    constexpr float centerX(float x, float width) noexcept {
        return x - (width / 2.0F);
    }

    constexpr float centerY(float y, float height) noexcept {
        return y - (height / 2.0F);
    }

    constexpr SDL_FPoint center(float x, float y, float width, float height) noexcept {
        return { .x = centerX(x, width), .y = centerY(y, height) };
    }

    constexpr float centerXInWindow(float width) noexcept {
        return centerX(TableCenterX, width);
    }

    constexpr float centerYInWindow(float height) noexcept {
        return centerY(TableCenterY, height);
    }

    constexpr SDL_FPoint centerInWindow(float width, float height) noexcept {
        return { .x = centerXInWindow(width), .y = centerYInWindow(height) };
    }

    enum class Anchor : uint8_t {
        CenterLeft,
        TopLeft,
        BottomLeft,
        CenterRight,
        TopRight,
        BottomRight,
        TopCenter,
        BottomCenter,
        Center,
        None
    };

    constexpr SDL_FPoint anchor(float x, float y, float width, float height, Anchor anchor) noexcept {
        switch (anchor) {
            case Anchor::TopLeft:
                return { .x = x, .y = y };
            case Anchor::BottomLeft:
                return { .x = x, .y = y + height };
            case Anchor::CenterLeft:
                return { .x = x, .y = centerY(y, height) };
            case Anchor::TopRight:
                return { .x = x + width, .y = y };
            case Anchor::BottomRight:
                return { .x = x + width, .y = y + height };
            case Anchor::CenterRight:
                return { .x = x + width, .y = centerY(y, height) };
            case Anchor::TopCenter:
                return { .x = centerX(x, width), .y = y };
            case Anchor::BottomCenter:
                return { .x = centerX(x, width), .y = y + height };
            case Anchor::Center:
                return center(x, y, width, height);
            case Anchor::None:
                return { .x = x, .y = y };
        }
    }

    constexpr SDL_FPoint anchorInRect(const SDL_FRect& rect, Anchor anchor) noexcept {
        return layout::anchor(rect.x, rect.y, rect.w, rect.h, anchor);
    }

}