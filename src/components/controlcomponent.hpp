#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP

#include "component.hpp"
enum class CONTROL_STATE { NORMAL = 0, AIRBORNE = 1, FALLING = 2, CLIMBING = 3 };
class ControlComponent : public IComponent {
  bool canClimbDown = false;
  bool canClimbUp = false;
  CONTROL_STATE controlState = CONTROL_STATE::NORMAL;

public:
  void setCanClimbDown(bool climbDown) { this->canClimbDown = climbDown; }
  bool getCanClimbDown() { return this->canClimbDown; }
  void setCanClimbUp(bool climbUp) { this->canClimbDown = climbUp; }
  bool getCanClimbUp() { return this->canClimbUp; }

  void setControlState(CONTROL_STATE state) { this->controlState = state; }
  CONTROL_STATE getControlState() const { return this->controlState; }
};
#endif
