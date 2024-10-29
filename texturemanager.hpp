#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include "game.hpp"

class TextureManager
{

public:
    static SDL_Texture *LoadTexture(const char *fileName);
    static void draw(SDL_Texture *tex, SDL_Rect source, SDL_Rect destination);
};

#endif
