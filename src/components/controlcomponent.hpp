#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP
#include "component.hpp"

enum class CONTROL_STATE { NORMAL = 0, AIRBORNE = 1, FALLING = 2, CLIMBING = 3 };
class ControlComponent : public IComponent {
  bool canClimb = false;
  CONTROL_STATE controlState = CONTROL_STATE::NORMAL;

public:
  void setCanClimb(bool canClimb) { this->canClimb = canClimb; }
  bool getCanClimb() const { return this->canClimb; }

  void setControlState(CONTROL_STATE state) { this->controlState = state; }
  CONTROL_STATE getControlState() const { return this->controlState; }
};
#endif
