#pragma once
#include "SDL_rect.h"
#ifndef ATTACKCOMPONENT_HPP
#define ATTACKCOMPONENT_HPP

struct AttackComponent {
  explicit AttackComponent(int attackFrame, int damage, SDL_Rect rightCollisionRect,
                           SDL_Rect leftCollisionRect)
      : attackFrames(attackFrame), damage(damage), rightCollisionRect(rightCollisionRect),
        leftCollisionRect(leftCollisionRect) {}
  int attackFrames;
  int damage;
  int currentAttackFrames = 0;
  SDL_Rect rightCollisionRect = {0, 0, 0, 0};
  SDL_Rect leftCollisionRect = {0, 0, 0, 0};

  bool attacking = false;
};

#endif
