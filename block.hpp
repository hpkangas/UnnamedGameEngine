#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobject.hpp"

class Block : public GameObject
{
    public:
    Block(const char *texturesheet, int x, int y, int dx, int dy);
    Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy);

    void update();
    void render();
    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect, collisionRect;

};

#endif