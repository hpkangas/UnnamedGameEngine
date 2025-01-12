#pragma once
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobject.hpp"

class Block : public GameObject
{
public:
    Block(const char *texturesheet, int x, int y, int dx, int dy);
    Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy);

    int xpos;
    int ypos;

    void update();
    void render(Camera camera);
    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect, collisionRect;

    int getXpos()
    {
        return this->xpos;
    }

    int getYpos()
    {
        return this->ypos;
    }

    void setXpos(int xpos)
    {
        this->xpos = xpos;
    }

    void setYpos(int ypos)
    {
        this->ypos = ypos;
    }
};

#endif