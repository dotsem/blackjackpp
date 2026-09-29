

#include "texture_manager.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include <string>

namespace presentation {
    SDL_Texture* TextureManager::load(const std::string& path) {
        auto it = textures_.find(path);
        if (it != textures_.end()) {
            return it->second.get();
        }

        SDL_Surface* surface = SDL_LoadPNG(path.c_str());
        if (surface == nullptr) {
            throw std::runtime_error("Failed to load image: " + path + " - " + SDL_GetError());
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_DestroySurface(surface);
        if (texture == nullptr) {
            throw std::runtime_error("Failed to create texture from surface: " + path + " - " + SDL_GetError());
        }
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        textures_.emplace(path, TextureHandle(texture));

        return texture;
    }

    SDL_Texture* TextureManager::get(const std::string& key) const {
        auto it = textures_.find(key);
        if (it != textures_.end()) {
            return it->second.get();
        }
        return nullptr;
    }
}