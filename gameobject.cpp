#include "gameobject.hpp"
#include "game.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_ttf.h>

GameObject::GameObject() {}
GameObject::~GameObject() {}

void GameObject::update() {}

void GameObject::render(Camera camera) {
  RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, SDL_FLIP_NONE, camera};
  TextureManager::draw(info);
}

// Background::Background(const char *texturesheet, int x, int y, int sizew, int sizeh) {
//   m_objTexture = TextureManager::LoadTexture(*texturesheet);
//   m_position = {x, y};
//   m_sourceRect.h = sizeh;
//   m_sourceRect.w = sizew;
// }

void Background::update() {

  // m_xpos -= m_xvel;

  // if (m_xpos <= -SCREENWIDTH)
  // {
  //     m_xpos = SCREENWIDTH * 2;
  // }

  // m_sourceRect.x = 0;
  // m_sourceRect.y = 0;

  // m_destinationRect.x = m_xpos;
  // m_destinationRect.y = m_ypos;
  // m_destinationRect.w = m_sourceRect.w;
  // m_destinationRect.h = m_sourceRect.h;
}

GameText::GameText(std::string textureText, int x, int y) {
  text = textureText;
  m_position = {x, y};
}

void GameText::update() {
  m_sourceRect.x = 0;
  m_sourceRect.y = 4;

  m_destinationRect.x = m_position.x;
  m_destinationRect.y = m_position.y;
  m_destinationRect.w = m_sourceRect.w;
  m_destinationRect.h = m_sourceRect.h;
}

void GameText::render(Camera camera) {
  // text = std::to_string(Game::score);
  SDL_Color white = {255, 255, 255};
  SDL_Surface *textSurface = TTF_RenderText_Solid(Game::gFont, text.c_str(), white);
  SDL_GetClipRect(textSurface, &m_destinationRect);
  m_objTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
  SDL_RenderCopy(Game::renderer, m_objTexture, NULL, &m_destinationRect);
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(m_objTexture);
}
