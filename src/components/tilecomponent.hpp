#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP
#include "component.hpp"
enum class TILE { NONE = 0, BLOCKING = 1, LADDER = 2 };

struct TileComponent : public IComponent {
  TILE tile;
  TileComponent() { tile = TILE::NONE; }
};

#endif
