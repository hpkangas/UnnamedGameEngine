#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP
#include "component.hpp"
#include "../utilities/utils.hpp"
enum class TILE { NONE = 0, SOLID = 1, SEMISOLID = 2, LADDER = 3, LADDERSTART = 4 };

struct TileComponent : public IComponent {
  explicit TileComponent(TILE tile, int x, int y) : tile(tile),tilePosition({x,y}){}
  TILE tile = TILE::NONE;
  Utils::Position tilePosition{0, 0};
};

#endif
