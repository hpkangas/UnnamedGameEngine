#ifndef GRAVITYCOMPONENT_HPP
#define GRAVITYCOMPONENT_HPP
#include "component.hpp"

enum class GRAVITY_STATE { GROUNDED = 0, AIRBORNE = 1, FALLING = 2 };

struct GravityComponent : public IComponent {

  int ID = GCID;

  GRAVITY_STATE gravityState = GRAVITY_STATE::AIRBORNE;

  GRAVITY_STATE getGravityState() const { return gravityState; }

  void setGravityState(const GRAVITY_STATE state) { gravityState = state; }
};

#endif
