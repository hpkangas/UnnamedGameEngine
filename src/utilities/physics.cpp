#include "physics.hpp"
#include "utils.hpp"
#include <cstdio>

using namespace Physics;
bool Physics::checkCollision(SDL_Rect const &a, SDL_Rect const &b) {
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

bool Physics::checkLeft(SDL_Rect a, SDL_Rect b) {
  int leftA, rightB;
  leftA = a.x;
  rightB = b.x + b.w;
  if (leftA >= rightB) {
    return false;
  }
  return true;
}

bool Physics::collisionDown(SDL_Rect const &a, Velocity const aVel,
                            SDL_Rect const &b, Velocity const bVel) {

  if (a.y + a.h <= b.y + b.h) {
    return a.y + a.h + aVel.y >= b.y + bVel.y &&
           a.x < b.x + b.w && a.x + a.w > b.x;
  }
  return false;
}

bool Physics::collisionUp(SDL_Rect const &a, Velocity const aVel,
                          SDL_Rect const &b, Velocity const bVel) {

  if (a.y >= b.y) {
    return a.y + aVel.y <= b.y + b.h + bVel.y &&
           a.x < b.x + b.w && a.x + a.w > b.x;
  }
  return false;
}

bool Physics::collisionLeft(SDL_Rect const &a, Velocity const aVel,
                            SDL_Rect const &b, Velocity const bVel) {

  if (a.x >= b.x + b.w) {
    return a.x + aVel.x < b.x + b.w + bVel.x &&
           a.y < b.y + b.h && a.y + a.h > b.y;
  }
  return false;
}

bool Physics::collisionRight(SDL_Rect const &a, Velocity const aVel,
                             SDL_Rect const &b, Velocity const bVel) {

  if (a.x + a.w <= b.x) {
    return a.x + a.w + aVel.x > b.x + bVel.x &&
           a.y < b.y + b.h && a.y + a.h > b.y;
  }
  return false;
}

bool Physics::collisionDownRight(SDL_Rect const &a, SDL_Rect const &b,
                                 Velocity const aVel) {

  const int cornerX = a.x + a.w + aVel.x;
  const int cornerY = a.y + a.h + aVel.y;
  const int bRight = b.x + b.w;
  const int bTop = b.y;

  const bool touchesLeft = cornerX == b.x && cornerY >= b.y && cornerY <= bTop;
  const bool touchesTop = cornerY == bTop && cornerX >= b.x && cornerX <= bRight;

  return touchesLeft || touchesTop;
}

bool Physics::collisionDownLeft(SDL_Rect const &a, SDL_Rect const &b,
                                Velocity const aVel) {

  const int cornerX = a.x + aVel.x;
  const int cornerY = a.y + a.h + aVel.y;
  const int bLeft = b.x;
  const int bTop = b.y;

  const bool touchesRight = cornerX == bLeft && cornerY >= b.y && cornerY <= bTop;
  const bool touchesTop = cornerY == bTop && cornerX >= bLeft && cornerX <= b.x + b.w;

  return touchesRight || touchesTop;
}

bool Physics::attackCollision(SDL_Rect const &a, SDL_Rect const &b) {
  return Physics::checkCollision(a, b);
  // Velocity playerVelocity = {0, 0};
  // return collisionDown(player, obj, playerVelocity) || collisionUp(player, obj, playerVelocity)
  // ||
  //  collisionLeft(player, obj, playerVelocity) || collisionRight(player, obj, playerVelocity);
} // namespace Physics
