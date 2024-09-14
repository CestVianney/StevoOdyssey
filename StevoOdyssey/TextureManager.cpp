#include "TextureManager.hpp"

std::unordered_map<std::string, SDL_Texture*> TextureManager::textureCache;
std::set<std::string> TextureManager::texturesToKeep = { "resources/characters/stevo.png" };

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    std::string fileStr(fileName);

    // V�rifier si la texture est d�j� dans le cache
    auto it = textureCache.find(fileStr);
    if (it != textureCache.end()) {
        return it->second;  // Retourner la texture d�j� charg�e
    }

    // Si la texture n'est pas encore charg�e, on la charge et l'ajoute au cache
    SDL_Surface* tmpSurface = IMG_Load(fileName);
    if (!tmpSurface) {
        std::cerr << "Failed to load texture: " << fileName << std::endl;
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    if (tex) {
        textureCache[fileStr] = tex;  // Ajouter la texture dans le cache
    }

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}

void TextureManager::ClearTextures() {
    for (auto it = textureCache.begin(); it != textureCache.end(); ) {
        if (texturesToKeep.find(it->first) == texturesToKeep.end()) {
            // La texture n'est pas dans la liste des exceptions, donc on peut la d�truire
            SDL_DestroyTexture(it->second);
            it = textureCache.erase(it); // Supprime l'entr�e du cache
        }
        else {
            ++it; // Passe � l'entr�e suivante
        }
    }
}

void TextureManager::AddTextureToKeep(const std::string& texturePath) {
    texturesToKeep.insert(texturePath);
}