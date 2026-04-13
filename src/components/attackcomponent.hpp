#pragma once
#include "SDL_rect.h"
#ifndef ATTACKCOMPONENT_HPP
#define ATTACKCOMPONENT_HPP

struct AttackComponent {
  explicit AttackComponent(int attackFrame, int damage, SDL_Rect collisionRect)
      : attackFrames(attackFrame), damage(damage), collisionRect(collisionRect) {}
  int attackFrames;
  int damage;
  int currentAttackFrames = 0;
  SDL_Rect collisionRect = {0, 0, 0, 0};

  bool attacking = false;
};

#endif
