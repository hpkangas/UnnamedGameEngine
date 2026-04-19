#ifndef AICOMPONENT_HPP
#define AICOMPONENT_HPP
#include "component.hpp"

enum class AI_STATE { DEAD = 0, WALKING = 1, ATTACKING = 2 };

struct AIComponent : public IComponent {
  bool on = true;
  AI_STATE state = AI_STATE::WALKING;
};

#endif
