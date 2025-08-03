#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP
#include "component.hpp"
enum class TILE { NONE = 0, SOLID = 1, SEMISOLID = 2, LADDER = 3 };

struct TileComponent : public IComponent {
  TileComponent() { tile = TILE::NONE; }
  TILE tile;
  TILE getTile(){return this->tile;}
};

#endif
