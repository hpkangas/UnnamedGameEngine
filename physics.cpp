#include "physics.hpp"

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

bool playerCollisionDown(const Player &player, const Block &obj) {

  if (player.collisionComponent.collisionRect.y + player.collisionComponent.collisionRect.h <=
      obj.collisionComponent.collisionRect.y) {
    {
      return (
          player.collisionComponent.collisionRect.y + player.collisionComponent.collisionRect.h +
                  player.transformComponent.getYvel() >=
              obj.collisionComponent.collisionRect.y &&
          player.collisionComponent.collisionRect.x <
              obj.collisionComponent.collisionRect.x + obj.collisionComponent.collisionRect.w - 1 &&
          player.collisionComponent.collisionRect.x + player.collisionComponent.collisionRect.w -
                  1 >
              obj.collisionComponent.collisionRect.x);
    }
  }
  return false;
}

bool playerCollisionUp(const Player &player, const Block &obj) {
  if (player.collisionComponent.collisionRect.y >=
      obj.collisionComponent.collisionRect.y + obj.collisionComponent.collisionRect.h) {

    return (
        player.collisionComponent.collisionRect.y + player.transformComponent.getYvel() <=
            obj.collisionComponent.collisionRect.y + obj.collisionComponent.collisionRect.h &&
        player.collisionComponent.collisionRect.x <
            obj.collisionComponent.collisionRect.x + obj.collisionComponent.collisionRect.w - 1 &&
        player.collisionComponent.collisionRect.x + player.collisionComponent.collisionRect.w - 1 >
            obj.collisionComponent.collisionRect.x);
  }
  return false;
}

bool playerCollisionLeft(const Player &player, const Block &obj) {
  if (player.collisionComponent.collisionRect.x >=
      obj.collisionComponent.collisionRect.x + obj.collisionComponent.collisionRect.w) {

    return (
        player.collisionComponent.collisionRect.x + player.transformComponent.getXvel() <
            obj.collisionComponent.collisionRect.x + obj.collisionComponent.collisionRect.w &&
        player.collisionComponent.collisionRect.y <
            obj.collisionComponent.collisionRect.y + obj.collisionComponent.collisionRect.h - 1 &&
        player.collisionComponent.collisionRect.y + player.collisionComponent.collisionRect.h - 1 >
            obj.collisionComponent.collisionRect.y);
  }
  return false;
}

bool playerCollisionRight(const Player &player, const Block &obj) {
  if (player.collisionComponent.collisionRect.x + player.collisionComponent.collisionRect.w <=
      obj.collisionComponent.collisionRect.x) {

    return (
        player.collisionComponent.collisionRect.x + player.collisionComponent.collisionRect.w +
                player.transformComponent.getXvel() >
            obj.collisionComponent.collisionRect.x &&
        player.collisionComponent.collisionRect.y <
            obj.collisionComponent.collisionRect.y + obj.collisionComponent.collisionRect.h - 1 &&
        player.collisionComponent.collisionRect.y + player.collisionComponent.collisionRect.h - 1 >
            obj.collisionComponent.collisionRect.y);
  }
  return false;
}
} // namespace Physics