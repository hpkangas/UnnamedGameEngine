#pragma once
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobject.hpp"

class Block : public GameObject
{
public:
    Block(const char *texturesheet, int x, int y, int dx, int dy);
    Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy);

    int m_xpos;
    int m_ypos;

    void update();
    void render(Camera camera);
    SDL_Texture *m_objTexture;
    SDL_Rect sourceRect, m_destinationRect, m_collisionRect;

    const int getXpos()
    {
        return m_xpos;
    }

    const int getYpos()
    {
        return m_ypos;
    }

    void setXpos(int xpos)
    {
        m_xpos = xpos;
    }

    void setYpos(int ypos)
    {
        m_ypos = ypos;
    }
};

#endif