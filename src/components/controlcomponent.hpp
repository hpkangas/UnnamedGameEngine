#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP

enum class CONTROL_STATE {
  DEAD = 0,
  NORMAL = 1,
  AIRBORNE = 2,
  FALLING = 3,
  CLIMBING = 4,
  ATTACKING = 5
};
struct ControlComponent {
  explicit ControlComponent(CONTROL_STATE state) : controlState(state) {}

  bool canClimb = false;
  CONTROL_STATE controlState = CONTROL_STATE::NORMAL;
};
#endif
