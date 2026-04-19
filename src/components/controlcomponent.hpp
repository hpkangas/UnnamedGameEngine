#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP
#include "component.hpp"

enum class CONTROL_STATE {
  DEAD = 0,
  NORMAL = 1,
  AIRBORNE = 2,
  FALLING = 3,
  CLIMBING = 4,
  ATTACKING = 5
};
struct ControlComponent : public IComponent {
  ControlComponent(CONTROL_STATE state, int id) : controlState(state) { parentID = id; }

  bool canClimb = false;
  CONTROL_STATE controlState = CONTROL_STATE::NORMAL;
};
#endif
