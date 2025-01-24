#pragma once
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobject.hpp"

class Block : public GameObject {
public:
  Block(SDL_Texture &texture, int x, int y, int dx, int dy);
  //Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy);

  void update() override;
  void render(Camera camera) override;

  int getXpos() const override { return  m_position.x; }

  int getYpos() const override { return m_position.y; }

  void setXpos(const int xpos) override { m_position.x = xpos; }

  void setYpos(const int ypos) override { m_position.y = ypos; }
};

#endif