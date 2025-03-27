#ifndef CONTROLCOMPONENT_HPP
#define CONTROLCOMPONENT_HPP
#include "../utils.hpp"

#include "component.hpp"
struct ControlComponent : public IComponent {
  bool canClimb = false;
  bool climbing = false;
  Utils::Position collidedTilePos = {0,0};

};
#endif
