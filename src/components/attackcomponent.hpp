#pragma once
#include "SDL_rect.h"
#ifndef ATTACKCOMPONENT_HPP
#define ATTACKCOMPONENT_HPP

struct AttackComponent {
  explicit AttackComponent(int attackFrame, int attackHitDetectionFrame, int damage, SDL_Rect collisionRect)
      : attackFrames(attackFrame),attackHitDetectionFrame(attackHitDetectionFrame), damage(damage), collisionRect(collisionRect) {}
  int attackFrames = 0;
  int attackHitDetectionFrame = 0;
  int damage = 0;
  int currentAttackFrames = 0;
  SDL_Rect collisionRect = {0, 0, 0, 0};

  bool attacking = false;
};

#endif
