#include "physics.hpp"
#include "components/collisioncomponent.hpp"
#include <iostream>

namespace Physics {
bool checkCollision(SDL_Rect &a, SDL_Rect b) {
  // The sides of the rectangles
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  // Calculate the sides of rect A
  leftA = a.x;
  rightA = a.x + a.w;
  topA = a.y;
  bottomA = a.y + a.h;

  // Calculate the sides of rect B
  leftB = b.x;
  rightB = b.x + b.w;
  topB = b.y;
  bottomB = b.y + b.h;

  // If any of the sides from A are outside of B
  if (bottomA <= topB) {
    a.y = topB;
    return false;
  }

  if (topA >= bottomB) {
    return false;
  }

  if (rightA <= leftB) {
    return false;
  }

  if (leftA >= rightB) {
    return false;
  }

  // If none of the sides from A are outside B
  return true;
}
bool checkLeft(SDL_Rect a, SDL_Rect b) {
  int leftA, rightB;
  leftA = a.x;
  rightB = b.x + b.w;
  if (leftA >= rightB) {
    return false;
  }
  return true;
}

bool playerCollisionDown(CollisionComponent &player, CollisionComponent &obj,
                         Velocity playerVelocity) {

  if (player.collisionRect.y + player.collisionRect.h <=
      obj.collisionRect.y + obj.collisionRect.h) {
    {
      return player.collisionRect.y + player.collisionRect.h + abs(playerVelocity.y) >=
                 obj.collisionRect.y &&
             player.collisionRect.x < obj.collisionRect.x + obj.collisionRect.w - 1 &&
             player.collisionRect.x + player.collisionRect.w - 1 > obj.collisionRect.x;
    }
  }
  return false;
}

bool playerCollisionUp(CollisionComponent &player, CollisionComponent &obj,
                       Velocity playerVelocity) {

  if (player.collisionRect.y >= obj.collisionRect.y) {
    return player.collisionRect.y - abs(playerVelocity.y) <=
               obj.collisionRect.y + obj.collisionRect.h &&
           player.collisionRect.x < obj.collisionRect.x + obj.collisionRect.w - 1 &&
           player.collisionRect.x + player.collisionRect.w - 1 > obj.collisionRect.x;
  }
  return false;
}

bool playerCollisionLeft(CollisionComponent &player, CollisionComponent &obj,
                         Velocity playerVelocity) {

  if (player.collisionRect.x >= obj.collisionRect.x + obj.collisionRect.w) {

    return player.collisionRect.x + playerVelocity.x < obj.collisionRect.x + obj.collisionRect.w &&
           player.collisionRect.y < obj.collisionRect.y + obj.collisionRect.h - 1 &&
           player.collisionRect.y + player.collisionRect.h - 1 > obj.collisionRect.y;
  }
  return false;
}

bool playerCollisionRight(CollisionComponent &player, CollisionComponent &obj,
                          Velocity playerVelocity) {

  if (player.collisionRect.x + player.collisionRect.w <= obj.collisionRect.x) {

    return player.collisionRect.x + player.collisionRect.w + playerVelocity.x >
               obj.collisionRect.x &&
           player.collisionRect.y < obj.collisionRect.y + obj.collisionRect.h - 1 &&
           player.collisionRect.y + player.collisionRect.h - 1 > obj.collisionRect.y;
  }
  return false;
}
} // namespace Physics
