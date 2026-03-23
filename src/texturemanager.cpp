#include "texturemanager.hpp"
#include "SDL_error.h"
#include "system.hpp"
#include <iostream>

namespace TextureManager {

SDL_Texture *LoadTexture(const char *fileName) {
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

} // namespace TextureManager
