#pragma once
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include <SDL2/SDL_image.h>

namespace TextureManager {
SDL_Texture *LoadTexture(const char *fileName);
};
// namespace TextureManager

#endif
