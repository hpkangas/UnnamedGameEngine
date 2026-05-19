#include "texturemanager.hpp"
#include "SDL_error.h"
#include "system.hpp"
#include <iostream>

SDL_Texture *TextureManager::loadTexture(const char *fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName);
  if (tempSurface != nullptr) {
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(System::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
  }
  std::cout << "ERROR: " << SDL_GetError() << std::endl;
  return nullptr;
}

void TextureManager::loadTextures() {
  std::cout << "Loading textures...\n";
  textures.emplace_back(TextureManager::loadTexture("assets/baleog.png"));
  textures.emplace_back(TextureManager::loadTexture("assets/tiles.png"));
  textures.emplace_back(TextureManager::loadTexture("assets/UI.png"));
  textures.emplace_back(TextureManager::loadTexture("assets/enemy.png"));
}

void TextureManager::clean() {
  for (SDL_Texture *texture : textures) {
      if (texture != nullptr) {
        SDL_DestroyTexture(texture);
      }
    }
    textures.clear();
}
