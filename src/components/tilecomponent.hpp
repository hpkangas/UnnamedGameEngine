#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP
#include "component.hpp"
enum Tile { NONE = 0, LADDER = 1 };

struct TileComponent : public IComponent {
  Tile tile;
  TileComponent() { tile = NONE; }
};

#endif
