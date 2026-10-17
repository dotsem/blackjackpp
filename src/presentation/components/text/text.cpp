#include "presentation/components/text/text.hpp"
#include "SDL3/SDL_error.h"
#include "SDL_fur_coat/surface.hpp"
#include <stdexcept>

namespace presentation {
    void Text::rebuild_texture() {
        if (is_empty()) {
            texture_.reset();
            return;
        }

        if (font_ == nullptr) {
            throw std::runtime_error("Font is not set. Cannot render text.");
        }

        sdl::Surface surface(TTF_RenderText_Blended(font_->get(), text_.c_str(), text_.length(), color_));
        if (!surface) {
            throw std::runtime_error(SDL_GetError());
        }

        texture_ = sdl::Texture::from_surface(renderer_, surface);
    }
}