#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP
#include "component.hpp"
enum class TILE { NONE = 0, SOLID = 1, SEMISOLID = 2, LADDER = 3, LADDERSTART = 4 };

struct TileComponent : public IComponent {
  TileComponent(TILE tile) { this->tile = tile; }
  TILE tile;
  TILE getTile() { return this->tile; }
};

#endif
