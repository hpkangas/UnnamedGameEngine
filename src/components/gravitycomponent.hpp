#ifndef GRAVITYCOMPONENT_HPP
#define GRAVITYCOMPONENT_HPP

enum class GRAVITY_STATE { GROUNDED = 0, AIRBORNE = 1, FALLING = 2 };

struct GravityComponent {
  GravityComponent(GRAVITY_STATE state) : gravityState(state) {}

  GRAVITY_STATE gravityState = GRAVITY_STATE::AIRBORNE;
};

#endif
