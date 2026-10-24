#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP
#include "../utilities/utils.hpp"
#include "../include/json/json.hpp"

enum class TILE { NONE = -1, EMPTY = 0, SOLID = 1, SEMISOLID = 2, LADDER = 3, LADDERSTART = 4 };

NLOHMANN_JSON_SERIALIZE_ENUM(TILE, {
  {TILE::NONE, "NONE"},
  {TILE::EMPTY, "EMPTY"},
  {TILE::SOLID, "SOLID"},
  {TILE::SEMISOLID, "SEMISOLID"},
  {TILE::LADDER, "LADDER"},
  {TILE::LADDERSTART, "LADDERSTART"},
})

struct TileComponent {
  explicit TileComponent(TILE tile, int x, int y) : tile(tile), tilePosition({x, y}) {}
  TILE tile = TILE::EMPTY;
  Utils::Position tilePosition{0, 0};
};

#endif
