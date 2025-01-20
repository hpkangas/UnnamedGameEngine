#include "texturemanager.hpp"
#include "game.hpp"
#include <iostream>

SDL_Texture *TextureManager::LoadTexture(const char *fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName);
  if (tempSurface != nullptr) {
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
  }
  std::cout << "ERROR LOADING TEXTURE " << fileName << std::endl;
  return nullptr;
}

void TextureManager::draw(const RenderInfo &RenderInfo) {
  SDL_Rect newDestination = RenderInfo.destinationRect;
  newDestination.x -= RenderInfo.camera.m_offset.x;
  newDestination.y -= RenderInfo.camera.m_offset.y;
  SDL_RenderCopyEx(Game::renderer, RenderInfo.objTexture, &RenderInfo.sourceRect, &newDestination,
                   0, nullptr, RenderInfo.flip);
}