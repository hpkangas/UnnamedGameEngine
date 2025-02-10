#include "transformcomponent.hpp"
#ifndef GRAVITYCOMPONENT_HPP
#define GRAVITYCOMPONENT_HPP

enum GravityState { GROUNDED = 0, INAIR = 1, FALLING = 2 };

struct GravityComponent {
  void gravity(TransformComponent &transformComponent);

  GravityState gravityState;

  GravityState getGravityState() const { return gravityState; }

  void setGravityState(const GravityState state) { gravityState = state; }
};

#endif