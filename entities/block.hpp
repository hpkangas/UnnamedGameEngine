#pragma once
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <SDL2/SDL_render.h>

class Block {
public:
  Block(int x, int y, int sourceX);

  Block createBlock1(int x, int y);
  Block createBlock2(int x, int y);
  Block createBlock3(int x, int y);

  TransformComponent tc{};
  RenderComponent rc{};
  SDL_Rect m_collisionRect{};

  void update();
  // void render(Camera camera) override;
};

#endif