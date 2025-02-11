#ifndef GRAVITYCOMPONENT_HPP
#define GRAVITYCOMPONENT_HPP
#include "component.hpp"
#include "transformcomponent.hpp"

enum GravityState { GROUNDED = 0, INAIR = 1, FALLING = 2 };

struct GravityComponent : public IComponent {
  void gravity(TransformComponent &transformComponent);

  GravityState gravityState;

  GravityState getGravityState() const { return gravityState; }

  void setGravityState(const GravityState state) { gravityState = state; }
};

#endif