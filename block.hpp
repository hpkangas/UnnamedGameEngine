#pragma once
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobject.hpp"

class Block : public GameObject {
public:
  Block(const char *texturesheet, int x, int y, int dx, int dy);
  Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy);

  Position m_position = {0, 0};
  Velocity m_velocity = {0, 0};

  void update();
  void render(Camera camera);
  SDL_Texture *m_objTexture;
  SDL_Rect sourceRect, m_destinationRect, m_collisionRect;

  const int getXpos() { return m_position.x; }

  const int getYpos() { return m_position.y; }

  void setXpos(int xpos) { m_position.x = xpos; }

  void setYpos(int ypos) { m_position.y = ypos; }
};

#endif