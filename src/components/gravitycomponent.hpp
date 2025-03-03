#ifndef GRAVITYCOMPONENT_HPP
#define GRAVITYCOMPONENT_HPP
#include "component.hpp"

enum GravityState { GROUNDED = 0, INAIR = 1, FALLING = 2 };

struct GravityComponent : public IComponent {

  int ID = GCID;

  GravityState gravityState = INAIR;

  GravityState getGravityState() const { return gravityState; }

  void setGravityState(const GravityState state) { gravityState = state; }
};

#endif
