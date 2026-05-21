#pragma once
#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP
#include "../utilities/utils.hpp"

struct TransformComponent {
  Utils::Position position{0, 0};
  Utils::Velocity velocity{0, 0};
  Utils::Position tilePosition{0, 0};
  Utils::Position initialTilePosition{0, 0};
  bool facingRight = true;

  TransformComponent(int x, int y, float velx, float vely)
      : position({x, y}), velocity({velx, vely}), tilePosition({x / 128, y / 128}),
        initialTilePosition({x / 128, y / 128}) {}
  TransformComponent(int x, int y) : TransformComponent(x, y, 0, 0) {}
};

#endif
