#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include "Game.hpp"

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void ClearTextures();
    static void AddTextureToKeep(const std::string& texturePath);


private:
    static std::unordered_map<std::string, SDL_Texture*> textureCache;
    static std::set<std::string> texturesToKeep;
};