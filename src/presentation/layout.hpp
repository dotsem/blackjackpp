#pragma once

namespace presentation::layout {
    inline constexpr float VirtualWidth = 1920.0F;
    inline constexpr float VirtualHeight = 1080.0F;
    inline constexpr float AspectRatio = VirtualWidth / VirtualHeight;
    inline constexpr float TableCenterX = VirtualWidth / 2.0F;
    inline constexpr float TableCenterY = VirtualHeight / 2.0F;
    inline constexpr float BorderPadding = 20.0F;

    constexpr float centerX(float width) noexcept {
        return TableCenterX - (width / 2.0F);
    }

    constexpr float centerY(float height) noexcept {
        return TableCenterY - (height / 2.0F);
    }
}