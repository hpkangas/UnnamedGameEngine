#include "physics.hpp"
#include <cstdio>
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

bool playerCollisionDown(SDL_Rect const &player, SDL_Rect const &obj,
                         Velocity const playerVelocity) {

  if (player.y + player.h <= obj.y + obj.h) {
      return player.y + player.h + playerVelocity.y >= obj.y &&
             player.x < obj.x + obj.w &&
             player.x + player.w > obj.x;
  }
  return false;
}

bool playerCollisionUp(SDL_Rect const &player, SDL_Rect const &obj,
                       Velocity const playerVelocity) {

  if (player.y >= obj.y) {
    return player.y + playerVelocity.y <= obj.y + obj.h &&
           player.x < obj.x + obj.w &&
           player.x + player.w > obj.x;
  }
  return false;
}

bool playerCollisionLeft(SDL_Rect const &player, SDL_Rect const &obj,
                         Velocity const playerVelocity) {

  if (player.x >= obj.x + obj.w) {
    return player.x + playerVelocity.x < obj.x + obj.w &&
           player.y < obj.y + obj.h &&
           player.y + player.h > obj.y;
  }
  return false;
}

bool playerCollisionRight(SDL_Rect const &player, SDL_Rect const &obj,
                          Velocity const playerVelocity) {

  if (player.x + player.w <= obj.x) {
    return player.x + player.w + playerVelocity.x > obj.x &&
           player.y < obj.y + obj.h &&
           player.y + player.h > obj.y;
  }
  return false;
}
} // namespace Physics
