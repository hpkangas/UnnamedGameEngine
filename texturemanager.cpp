#include "texturemanager.hpp"
#include "gamesystem.hpp"
#include <iostream>

namespace TextureManager {

SDL_Texture *LoadTexture(const char *fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName);
  if (tempSurface != nullptr) {
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(GameSystem::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
  }
  std::cout << "ERROR LOADING TEXTURE " << fileName << std::endl;
  return nullptr;
}

// void draw(const RenderInfo &RenderInfo, const Camera &camera) {
//   SDL_Rect newDestination = RenderInfo.destinationRect;
//   newDestination.x -= camera.m_offset.x;
//   newDestination.y -= camera.m_offset.y;
//   SDL_RenderCopyEx(GameSystem::renderer, RenderInfo.objTexture, &RenderInfo.sourceRect, &newDestination,
//                    0, nullptr, RenderInfo.flip);
// }
} // namespace TextureManager