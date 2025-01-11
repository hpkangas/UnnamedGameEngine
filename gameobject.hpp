#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "game.hpp"

class GameObject
{
public:
    GameObject(const char *texturesheet, int x, int y, int xvel, int yvel, int sizew, int sizeh);
    GameObject(const char *texturesheet, int x, int y, int sizew, int sizeh);
    GameObject();
    ~GameObject();

    virtual void update();
    virtual void render();

    int xpos = 0;
    int ypos = 0;

    int xvel = 0;
    int yvel = 0;

    void printCoordinates(SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_Rect collisionRect);

    virtual int getXpos()
    {
        return this->xpos;
    }

    virtual int getYpos()
    {
        return this->ypos;
    }

    virtual void setXpos(int xpos)
    {
        this->xpos = xpos;
    }

    virtual void setYpos(int ypos)
    {
        this->ypos = ypos;
    }

    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect, collisionRect;

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