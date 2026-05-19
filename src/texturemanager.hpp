#pragma once
#include <vector>
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include <SDL2/SDL_image.h>

struct TextureManager {
  inline static std::vector<SDL_Texture *> textures;
  static void loadTextures();
  static void clean();
  static SDL_Texture *loadTexture(const char *fileName);
};
// namespace TextureManager

#endif
