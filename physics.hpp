#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "block.hpp"
#include "player.hpp"
#include <SDL2/SDL.h>
//namespace?
struct Physics {

  static bool checkCollision(SDL_Rect &a, SDL_Rect b) {
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
  static bool checkLeft(SDL_Rect a, SDL_Rect b) {
    int leftA, rightB;
    leftA = a.x;
    rightB = b.x + b.w;
    if (leftA >= rightB) {
      return false;
    }
    return true;
  }

  static bool playerCollisionDown(const Player& player, const Block& obj) {

    if (player.m_collisionRect.y + player.m_collisionRect.h <= obj.m_collisionRect.y) {
      {
        return (player.m_collisionRect.y + player.m_collisionRect.h + player.getYvel() >=
                    obj.m_collisionRect.y &&
                player.m_collisionRect.x < obj.m_collisionRect.x + obj.m_collisionRect.w - 1 &&
                player.m_collisionRect.x + player.m_collisionRect.w - 1 > obj.m_collisionRect.x);
      }
    }
    return false;
  }

  static bool playerCollisionUp(const Player& player, const Block &obj) {
    if (player.m_collisionRect.y >= obj.m_collisionRect.y + obj.m_collisionRect.h) {

      return (player.m_collisionRect.y + player.getYvel() <=
                  obj.m_collisionRect.y + obj.m_collisionRect.h &&
              player.m_collisionRect.x < obj.m_collisionRect.x + obj.m_collisionRect.w - 1 &&
              player.m_collisionRect.x + player.m_collisionRect.w - 1 > obj.m_collisionRect.x);
    }
    return false;
  }

  static bool playerCollisionLeft(const Player& player, const Block& obj) {
    if (player.m_collisionRect.x >= obj.m_collisionRect.x + obj.m_collisionRect.w) {

      return (player.m_collisionRect.x + player.getXvel() <
                  obj.m_collisionRect.x + obj.m_collisionRect.w &&
              player.m_collisionRect.y < obj.m_collisionRect.y + obj.m_collisionRect.h - 1 &&
              player.m_collisionRect.y + player.m_collisionRect.h - 1 > obj.m_collisionRect.y);
    }
    return false;
  }

  static bool playerCollisionRight(const Player& player, const Block& obj) {
    if (player.m_collisionRect.x + player.m_collisionRect.w <= obj.m_collisionRect.x) {

      return (player.m_collisionRect.x + player.m_collisionRect.w + player.getXvel() >
                  obj.m_collisionRect.x &&
              player.m_collisionRect.y < obj.m_collisionRect.y + obj.m_collisionRect.h - 1 &&
              player.m_collisionRect.y + player.m_collisionRect.h - 1 > obj.m_collisionRect.y);
    }
    return false;
  }
};
#endif