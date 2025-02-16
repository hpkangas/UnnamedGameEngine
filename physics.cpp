#include "physics.hpp"
#include "components/transformcomponent.hpp"
#include "entities/entity.hpp"

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

bool playerCollisionDown(Entity &player, Entity &obj) {
  auto playerc = *player.GetComponent<CollisionComponent>();
  auto objc = *obj.GetComponent<CollisionComponent>();

  if (playerc.collisionRect.y + playerc.collisionRect.h <= objc.collisionRect.y) {
    {
      return (playerc.collisionRect.y + playerc.collisionRect.h +
                      player.GetComponent<TransformComponent>()->getYvel() >=
                  objc.collisionRect.y &&
              playerc.collisionRect.x < objc.collisionRect.x + objc.collisionRect.w - 1 &&
              playerc.collisionRect.x + playerc.collisionRect.w - 1 > objc.collisionRect.x);
    }
  }
  return false;
}

bool playerCollisionUp(Entity &player, Entity &obj) {
  auto playerc = player.GetComponent<CollisionComponent>();
  auto objc = obj.GetComponent<CollisionComponent>();

  if (playerc->collisionRect.y >= objc->collisionRect.y + objc->collisionRect.h) {

    return (playerc->collisionRect.y + player.GetComponent<TransformComponent>()->getYvel() <=
                objc->collisionRect.y + objc->collisionRect.h &&
            playerc->collisionRect.x < objc->collisionRect.x + objc->collisionRect.w - 1 &&
            playerc->collisionRect.x + playerc->collisionRect.w - 1 > objc->collisionRect.x);
  }
  return false;
}

bool playerCollisionLeft(Entity &player, Entity &obj) {
  auto playerc = player.GetComponent<CollisionComponent>();
  auto objc = obj.GetComponent<CollisionComponent>();

  if (playerc->collisionRect.x >= objc->collisionRect.x + objc->collisionRect.w) {

    return (playerc->collisionRect.x + player.GetComponent<TransformComponent>()->getXvel() <
                objc->collisionRect.x + objc->collisionRect.w &&
            playerc->collisionRect.y < objc->collisionRect.y + objc->collisionRect.h - 1 &&
            playerc->collisionRect.y + playerc->collisionRect.h - 1 > objc->collisionRect.y);
  }
  return false;
}

bool playerCollisionRight(Entity &player, Entity &obj) {
  auto playerc = player.GetComponent<CollisionComponent>();
  auto objc = obj.GetComponent<CollisionComponent>();

  if (playerc->collisionRect.x + playerc->collisionRect.w <= objc->collisionRect.x) {

    return (playerc->collisionRect.x + playerc->collisionRect.w +
                    player.GetComponent<TransformComponent>()->getXvel() >
                objc->collisionRect.x &&
            playerc->collisionRect.y < objc->collisionRect.y + objc->collisionRect.h - 1 &&
            playerc->collisionRect.y + playerc->collisionRect.h - 1 > objc->collisionRect.y);
  }
  return false;
}
} // namespace Physics