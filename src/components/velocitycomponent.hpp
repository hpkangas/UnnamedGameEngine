#ifndef VELOCITYCOMPONENT_HPP
#define VELOCITYCOMPONENT_HPP
#include "../utilities/utils.hpp"

constexpr float playerJumpSpeed = -16.0f;
constexpr float playerSpeed = 1.0f;
constexpr float playerSpeedMax = 8.0f;
constexpr float playerClimbingSpeedMax = 4.0f;

struct VelocityComponent {
  Utils::Velocity velocity{0, 0};

  VelocityComponent(float x, float y) : velocity({x, y}) {}


};

#endif
