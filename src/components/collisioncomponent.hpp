#ifndef COLLISIONCOMPONENT_HPP
#define COLLISIONCOMPONENT_HPP
#include "../include/entt/entt.hpp"
#include "../utilities/utils.hpp"
#include "SDL_rect.h"
#include "tilecomponent.hpp"
#include <vector>

struct Collision {
  bool collision = false;
  TileVector tileTypes;
  Utils::Position positionDifference = {0, 0};
  Utils::Velocity velocity = {0, 0};

  void reset() {
    collision = false;
    tileTypes.clear();
    positionDifference = {0, 0};
    velocity = {0, 0};
  }
};

struct CollisionComponent {
  explicit CollisionComponent(SDL_Rect collision) : collisionRect(collision) {}
  SDL_Rect collisionRect{0, 0, 0, 0};
  Utils::Position center = {(collisionRect.x + collisionRect.w + collisionRect.w) / 2,
                            (collisionRect.y + collisionRect.h + collisionRect.h) / 2};

  std::vector<entt::entity> collisionIds;

  Collision up;
  Collision down;
  Collision left;
  Collision right;

  Collision downRight;
  Collision downLeft;
};

#endif
