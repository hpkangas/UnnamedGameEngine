#include "gameobject.hpp"
#ifndef BLOCK.HPP
#define BLOCK .HPP

class Block : public GameObject
{
    public:
    Block(const char *texturesheet, int x, int y, int dx, int dy);

    void update();
    void render();
    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect, collisionRect;

};

#endif