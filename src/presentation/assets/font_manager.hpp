#pragma once

#include "SDL_fur_coat/font.hpp"
#include <array>
#include <cstdint>
#include <string>

namespace presentation {
    enum class FontSize : std::uint8_t {
        xs,
        sm,
        md,
        lg,
        xl,
        xxl,
        xxxl,
        count,
    };

    class FontManager {
    public:
        explicit FontManager(const std::string& font_path)
            : fonts_{
                sdl::Font(font_path, 16.0F),
                sdl::Font(font_path, 20.0F),
                sdl::Font(font_path, 28.0F),
                sdl::Font(font_path, 36.0F),
                sdl::Font(font_path, 60.0F),
                sdl::Font(font_path, 80.0F),
                sdl::Font(font_path, 96.0F)
            } {
        }

        [[nodiscard]] const sdl::Font& get(FontSize size) const noexcept {
            return fonts_.at(static_cast<std::uint8_t>(size));
        }

        [[nodiscard]] sdl::Font& get(FontSize size) noexcept {
            return fonts_.at(static_cast<std::uint8_t>(size));
        }

        [[nodiscard]] const sdl::Font& xs() const noexcept {
            return get(FontSize::xs);
        }

        [[nodiscard]] const sdl::Font& sm() const noexcept {
            return get(FontSize::sm);
        }

        [[nodiscard]] const sdl::Font& md() const noexcept {
            return get(FontSize::md);
        }

        [[nodiscard]] const sdl::Font& lg() const noexcept {
            return get(FontSize::lg);
        }

        [[nodiscard]] const sdl::Font& xl() const noexcept {
            return get(FontSize::xl);
        }

        [[nodiscard]] const sdl::Font& xxl() const noexcept {
            return get(FontSize::xxl);
        }

        [[nodiscard]] const sdl::Font& xxxl() const noexcept {
            return get(FontSize::xxxl);
        }

    private:
        std::array<sdl::Font, static_cast<std::uint8_t>(FontSize::count)> fonts_;
    };
}