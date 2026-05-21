#ifndef COLLISIONCOMPONENT_HPP
#define COLLISIONCOMPONENT_HPP
#include "../utilities/utils.hpp"
#include "SDL_rect.h"
#include "tilecomponent.hpp"

struct CollisionComponent {
  explicit CollisionComponent(SDL_Rect collision) : collisionRect(collision) {}
  SDL_Rect collisionRect{0, 0, 0, 0};
  Utils::Position center = {(collisionRect.x + collisionRect.w + collisionRect.w) / 2,
                            (collisionRect.y + collisionRect.h + collisionRect.h) / 2};

  bool collisionUp = false;
  bool collisionDown = false;
  bool collisionLeft = false;
  bool collisionRight = false;

  TILE collisionTileLeft = TILE::NONE;
  TILE collisionTileRight = TILE::NONE;
  TILE collisionTileDown = TILE::NONE;
  TILE collisionTileUp = TILE::NONE;

  Utils::Position collidedDownTilePos = {0, 0};
  Utils::Position collidedUpTilePos = {0, 0};

  int xposDif = 0;
  int yposDif = 0;
};

#endif
