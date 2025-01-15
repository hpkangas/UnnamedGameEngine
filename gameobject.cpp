#include "gameobject.hpp"
#include "texturemanager.hpp"
#include "constants.hpp"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include "game.hpp"

GameObject::GameObject(const char *texturesheet, int x, int y, int xvelocity, int yvelocity, int sizew, int sizeh)
{
    m_objTexture = TextureManager::LoadTexture(texturesheet);
    m_xpos = x;
    m_ypos = y;
    m_xvel = xvelocity;
    m_yvel = yvelocity;
}

GameObject::GameObject(const char *texturesheet, int x, int y, int sizew, int sizeh)
{
    m_objTexture = TextureManager::LoadTexture(texturesheet);
    m_xpos = x;
    m_ypos = y;
    m_sourceRect.h = sizeh;
    m_sourceRect.w = sizew;
}

GameObject::GameObject() {}
GameObject::~GameObject() {}

void GameObject::update()
{
    m_sourceRect.x = 0;
    m_sourceRect.y = 0;

    m_destinationRect.x = m_xpos;
    m_destinationRect.y = m_ypos;
    m_destinationRect.w = m_sourceRect.w;
    m_destinationRect.h = m_sourceRect.h;
}

void GameObject::render(Camera camera)
{
    TextureManager::draw(this->m_objTexture, this->m_sourceRect, this->m_destinationRect, camera);
}

void GameObject::printCoordinates(SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_Rect m_collisionRect)
{
    printf("Sprite drawn from x %d, y %d, h %d, w %d \n", sourceRect.x, sourceRect.y, sourceRect.h, sourceRect.w);
    printf("Sprite drawn to x %d, y %d, h %d, w %d \n", destinationRect.x, destinationRect.y, destinationRect.h, destinationRect.w);
    printf("Collision rect in x %d, y %d, h %d, w %d \n", m_collisionRect.x, m_collisionRect.y, m_collisionRect.h, m_collisionRect.w);
}

Background::Background(const char *texturesheet, int x, int y, int sizew, int sizeh, int xvelocity)
{
    m_objTexture = TextureManager::LoadTexture(texturesheet);
    m_xpos = x;
    m_ypos = y;
    m_xvel = xvelocity;
    m_sourceRect.h = sizeh;
    m_sourceRect.w = sizew;
}

void Background::update()
{

    m_xpos -= m_xvel;

    if (m_xpos <= -SCREENWIDTH)
    {
        m_xpos = SCREENWIDTH * 2;
    }

    m_sourceRect.x = 0;
    m_sourceRect.y = 0;

    m_destinationRect.x = m_xpos;
    m_destinationRect.y = m_ypos;
    m_destinationRect.w = m_sourceRect.w;
    m_destinationRect.h = m_sourceRect.h;
}

GameText::GameText(std::string textureText, int x, int y, int width, int height)
{
    text = textureText;

    m_xpos = x;
    m_ypos = y;
    m_sourceRect.h = height;
    m_sourceRect.w = width;
}
GameText::GameText(int x, int y, int width, int height)
{
    m_xpos = x;
    m_ypos = y;
    m_sourceRect.h = height;
    m_sourceRect.w = width;
}

GameText::GameText(std::string textureText, int x, int y)
{
    text = textureText;

    m_xpos = x;
    m_ypos = y;
}
GameText::GameText(int x, int y)
{
    m_xpos = x;
    m_ypos = y;
}

void GameText::update()
{
    m_sourceRect.x = 0;
    m_sourceRect.y = 4;

    m_destinationRect.x = m_xpos;
    m_destinationRect.y = m_ypos;
    m_destinationRect.w = m_sourceRect.w;
    m_destinationRect.h = m_sourceRect.h;
}

void GameText::render()
{
    // text = std::to_string(Game::score);
    SDL_Color white = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(Game::gFont, text.c_str(), white);
    SDL_GetClipRect(textSurface, &m_destinationRect);
    m_objTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_RenderCopy(Game::renderer, m_objTexture, NULL, &m_destinationRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(m_objTexture);
}
