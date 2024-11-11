#include "gameobject.hpp"
#include "texturemanager.hpp"
#include "constants.hpp"
#include <iostream>

GameObject::GameObject(const char *texturesheet, int x, int y, int xvelocity, int yvelocity, int sizew, int sizeh)
{
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
    xvel = xvelocity;
    yvel = yvelocity;
}

GameObject::GameObject(const char *texturesheet, int x, int y, int sizew, int sizeh)
{
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
    sourceRect.h = sizeh;
    sourceRect.w = sizew;
}

GameObject::GameObject() {}
GameObject::~GameObject() {}

void GameObject::update()
{
    sourceRect.x = 0;
    sourceRect.y = 0;

    destinationRect.x = xpos;
    destinationRect.y = ypos;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;
}

void GameObject::render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &sourceRect, &destinationRect);
}

void GameObject::printCoordinates(SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_Rect collisionRect)
{
    printf("Sprite drawn from x %d, y %d, h %d, w %d \n", sourceRect.x, sourceRect.y, sourceRect.h, sourceRect.w );
    printf("Sprite drawn to x %d, y %d, h %d, w %d \n", destinationRect.x, destinationRect.y, destinationRect.h, destinationRect.w );
    printf("Collision rect in x %d, y %d, h %d, w %d \n", collisionRect.x, collisionRect.y, collisionRect.h, collisionRect.w );
}

Background::Background(const char *texturesheet, int x, int y, int sizew, int sizeh, int xvelocity)
{
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
    xvel = xvelocity;
    sourceRect.h = sizeh;
    sourceRect.w = sizew;
}

void Background::update()
{

    xpos -= xvel;

    if (xpos <= -SCREENWIDTH)
    {
        xpos = SCREENWIDTH * 2;
    }

    sourceRect.x = 0;
    sourceRect.y = 0;

    destinationRect.x = xpos;
    destinationRect.y = ypos;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;
}

GameText::GameText(std::string textureText, int x, int y, int width, int height)
{
    text = textureText;

    xpos = x;
    ypos = y;
    sourceRect.h = height;
    sourceRect.w = width;
}
GameText::GameText(int x, int y, int width, int height)
{
    xpos = x;
    ypos = y;
    sourceRect.h = height;
    sourceRect.w = width;
}

GameText::GameText(std::string textureText, int x, int y)
{
    text = textureText;

    xpos = x;
    ypos = y;
}
GameText::GameText(int x, int y)
{
    xpos = x;
    ypos = y;
}

void GameText::update()
{
    sourceRect.x = 0;
    sourceRect.y = 4;

    destinationRect.x = xpos;
    destinationRect.y = ypos;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;
}

void GameText::render()
{
    // text = std::to_string(Game::score);
    SDL_Color white = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(Game::gFont, text.c_str(), white);
    SDL_GetClipRect(textSurface, &destinationRect);
    objTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_RenderCopy(Game::renderer, objTexture, NULL, &destinationRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(objTexture);
}


