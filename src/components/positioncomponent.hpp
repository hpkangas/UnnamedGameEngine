#pragma once
#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP
#include "../utilities/utils.hpp"

struct PositionComponent {
  Utils::Position position{0, 0};
  Utils::Position tilePosition{0, 0};
  Utils::Position initialTilePosition{0, 0};
  bool facingRight = true;

  PositionComponent(int x, int y)
      : position({x, y}), tilePosition({x / 128, y / 128}),
        initialTilePosition({x / 128, y / 128}) {}
};

#endif
