#include "gameobject.hpp"
#include <SDL2/SDL_ttf.h>

void GameObject::update() {}

// void GameObject::render(const Camera camera) {
//   const RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, SDL_FLIP_NONE, camera};
//   TextureManager::draw(info);
// }

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
  tc.position = {x, y};
}

void GameText::update() {
  rc.sourceRect.x = 0;
  rc.sourceRect.y = 4;

  rc.destinationRect.x = tc.position.x;
  rc.destinationRect.y = tc.position.y;
  rc.destinationRect.w = rc.sourceRect.w;
  rc.destinationRect.h = rc.sourceRect.h;
}

// void GameText::render(Camera camera) {
//   // text = std::to_string(Game::score);
//   SDL_Color white = {255, 255, 255};
//   SDL_Surface *textSurface = TTF_RenderText_Solid(Game::gFont, text.c_str(), white);
//   SDL_GetClipRect(textSurface, &rc.m_destinationRect);
//   m_objTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
//   SDL_RenderCopy(Game::renderer, m_objTexture, nullptr, &rc.m_destinationRect);
//   SDL_FreeSurface(textSurface);
//   SDL_DestroyTexture(m_objTexture);
// }
