#pragma once
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include <SDL2/SDL_image.h>
#include "camera.hpp"

class TextureManager
{

public:
    static SDL_Texture *LoadTexture(const char *fileName);
    static void draw(SDL_Texture *tex, SDL_Rect source, SDL_Rect destination, Camera camera);
    static void draw(SDL_Texture* objTexture, SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_RendererFlip flip, Camera camera);
};

#endif
