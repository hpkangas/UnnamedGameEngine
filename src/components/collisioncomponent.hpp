#ifndef COLLISIONCOMPONENT_HPP
#define COLLISIONCOMPONENT_HPP
#include "../include/entt/entt.hpp"
#include "../utilities/utils.hpp"
#include "SDL_rect.h"
#include "tilecomponent.hpp"
#include <vector>

struct CollisionComponent {
  explicit CollisionComponent(SDL_Rect collision) : collisionRect(collision) {}
  SDL_Rect collisionRect{0, 0, 0, 0};
  Utils::Position center = {(collisionRect.x + collisionRect.w + collisionRect.w) / 2,
                            (collisionRect.y + collisionRect.h + collisionRect.h) / 2};

  std::vector<entt::entity> collisionIds;

  bool tileCollisionUp = false;
  bool tileCollisionDown = false;
  bool tileCollisionLeft = false;
  bool tileCollisionRight = false;

  TILE collisionTileTypeLeft = TILE::EMPTY;
  TILE collisionTileTypeRight = TILE::EMPTY;
  TILE collisionTileTypeDown = TILE::EMPTY;
  TILE collisionTileTypeUp = TILE::EMPTY;

  Utils::Position collidedDownTilePos = {0, 0};
  Utils::Position collidedUpTilePos = {0, 0};

  int xposDif = 0;
  int yposDif = 0;
};

#endif
