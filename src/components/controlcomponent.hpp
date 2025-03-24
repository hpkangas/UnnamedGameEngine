#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP

#include "component.hpp"
struct ControlComponent : public IComponent {
  bool canClimb = false;
  bool climbing = false;
};
#endif
