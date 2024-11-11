#ifndef BLOCK.HPP
#define BLOCK .HPP
#include "gameobject.hpp"

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