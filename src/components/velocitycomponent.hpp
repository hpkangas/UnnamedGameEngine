#ifndef VELOCITYCOMPONENT_HPP
#define VELOCITYCOMPONENT_HPP
#include "../utilities/utils.hpp"

struct VelocityComponent {
  Utils::Velocity velocity{0, 0};

  VelocityComponent(float x, float y) : velocity({x, y}) {}
};

#endif
