#pragma once
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include <SDL2/SDL_image.h>
#include "camera.hpp"
#include <string>

class GameObject
{
public:
    GameObject(const char *texturesheet, int x, int y, int m_xvel, int m_yvel, int sizew, int sizeh);
    GameObject(const char *texturesheet, int x, int y, int sizew, int sizeh);
    GameObject();
    ~GameObject();

    virtual void update();
    virtual void render(Camera camera);

    int m_xpos;
    int m_ypos;

    int m_xvel;
    int m_yvel;

    void printCoordinates(SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_Rect collisionRect);

    const virtual int getXpos()
    {
        return m_xpos;
    }

    const virtual int getYpos()
    {
        return m_ypos;
    }

    virtual void setXpos(int xpos)
    {
        m_xpos = xpos;
    }

    virtual void setYpos(int ypos)
    {
        m_ypos = ypos;
    }

    SDL_Texture *m_objTexture;
    SDL_Rect m_sourceRect, m_destinationRect, m_collisionRect;

private:
};

class Background : public GameObject
{
public:
    Background(const char *texturesheet, int x, int y, int sizew, int sizeh, int xvelocity);
    void update();
};

class GameText : public GameObject
{
public:
    GameText(std::string textureText, int x, int y);
    GameText(int x, int y);
    GameText(std::string textureText, int x, int y, int width, int height);
    GameText(int x, int y, int width, int height);
    void update();
    void render();

    std::string text;
};

#endif