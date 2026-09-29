#pragma once
#include "SDL3/SDL_render.h"
#include "SDL_fur_coat/resource.hpp"
#include <string>
#include <unordered_map>

namespace presentation {

    using TextureHandle = sdl::UniqueResource<SDL_Texture, SDL_DestroyTexture>;

    class TextureManager {
    public:
        explicit TextureManager(SDL_Renderer* renderer)
            : renderer_(renderer) {
        }

        ~TextureManager() = default;

        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        TextureManager(TextureManager&&) noexcept = default;
        TextureManager& operator=(TextureManager&&) noexcept = default;

        SDL_Texture* load(const std::string& path);
        [[nodiscard]] SDL_Texture* get(const std::string& key) const;

    private:
        SDL_Renderer* renderer_;
        std::unordered_map<std::string, TextureHandle> textures_;
    };
}