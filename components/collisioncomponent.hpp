#include "SDL_rect.h"
#include "component.hpp"
#ifndef COLLISIONCOMPONENT_HPP
#define COLLISIONCOMPONENT_HPP
struct CollisionComponent : public IComponent {
  SDL_Rect collisionRect{0, 0, 0, 0};

  int yposDif = 0;
  int xposDif = 0;

  bool collisionUp = false;
  bool collisionDown = false;
  bool collisionLeft = false;
  bool collisionRight = false;

  int getXposDifferenceWithCollisionObject() const { return xposDif; }

  int getYposDifferenceWithCollisionObject() const { return yposDif; }

  bool getCollisionDown() const { return collisionDown; }

  bool getCollisionUp() const { return collisionUp; }

  bool getCollisionRight() const { return collisionRight; }

  bool getCollisionLeft() const { return collisionLeft; }

  void setCollisionDown(const bool collision) { collisionDown = collision; }

  void setCollisionUp(const bool collision) { collisionUp = collision; }

  void setCollisionRight(const bool collision) { collisionRight = collision; }

  void setCollisionLeft(const bool collision) { collisionLeft = collision; }

  void setXposDifferenceWithCollisionObject(const int dif) { xposDif = dif; }

  void setYposDifferenceWithCollisionObject(const int dif) { yposDif = dif; }
};

#endif