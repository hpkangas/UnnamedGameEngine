#pragma once
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include "camera.hpp"
#include <SDL2/SDL_image.h>
struct RenderInfo {
  SDL_Texture *objTexture{};
  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  Camera camera;
};

class TextureManager {

public:
  static SDL_Texture *LoadTexture(const char *fileName);
  static void draw(const RenderInfo &RenderInfo);
};

#endif
