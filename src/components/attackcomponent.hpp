#pragma once
#ifndef ATTACKCOMPONENT_HPP
#define ATTACKCOMPONENT_HPP

struct AttackComponent {
  explicit AttackComponent(int attackFrame, int damage)
      : attackFrames(attackFrame), damage(damage) {}
  int attackFrames;
  int damage;
  int currentAttackFrames = 0;
};

#endif
