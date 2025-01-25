#pragma once
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobject.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_render.h>

class Block : public GameObject {
public:
  Block(int x, int y, int sourceX);

  Block createBlock1(int x, int y);
  Block createBlock2(int x, int y);
  Block createBlock3(int x, int y);

  SDL_Texture *m_objTexture = TextureManager::LoadTexture("assets/tiles.png");

  void update() override;
  void render(Camera camera) override;

  int getXpos() const override { return m_position.x; }

  int getYpos() const override { return m_position.y; }

  void setXpos(const int xpos) override { m_position.x = xpos; }

  void setYpos(const int ypos) override { m_position.y = ypos; }
};

#endif