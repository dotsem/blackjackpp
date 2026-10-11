#pragma once

#include "SDL3/SDL_error.h"
#include "SDL_fur_coat/resource.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>

namespace sdl {
    using FontHandle = UniqueResource<TTF_Font, TTF_CloseFont>;

    class Font {
    public:
        Font(const std::string& path, float pt_size)
            : handle_(TTF_OpenFont(path.c_str(), pt_size)) {
            if (!handle_) {
                throw std::runtime_error(SDL_GetError());
            }
        }

        [[nodiscard]] TTF_Font* get() const noexcept {
            return handle_.get();
        }

    private:
        FontHandle handle_;
    };
}
