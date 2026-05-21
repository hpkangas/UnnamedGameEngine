#ifndef AICOMPONENT_HPP
#define AICOMPONENT_HPP

enum class AI_STATE { DEAD = 0, WALKING = 1, ATTACKING = 2 };

struct AIComponent {
  bool on = true;
  AI_STATE state = AI_STATE::WALKING;
};

#endif
