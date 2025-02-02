#pragma once
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "player.hpp"
#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <SDL2/SDL_render.h>

class Block {
public:
  Block(int x, int y, int sourceX);

  Block createBlock1(int x, int y);
  Block createBlock2(int x, int y);
  Block createBlock3(int x, int y);

  TransformComponent transformComponent{};
  RenderComponent renderComponent{};
  CollisionComponent collisionComponent{};

  void update();
  // void render(Camera camera) override;
};

#endif