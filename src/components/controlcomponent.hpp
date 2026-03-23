#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP
#include "component.hpp"

enum class CONTROL_STATE { NORMAL = 0, AIRBORNE = 1, FALLING = 2, CLIMBING = 3 };
struct ControlComponent : public IComponent {
  ControlComponent(CONTROL_STATE state, int id): controlState(state) {
    parentID = id;
  }

  bool canClimb = false;
  CONTROL_STATE controlState = CONTROL_STATE::NORMAL;

};
#endif
