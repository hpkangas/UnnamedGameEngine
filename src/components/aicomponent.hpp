#ifndef AICOMPONENT_HPP
#define AICOMPONENT_HPP

enum class AI_STATE { DEAD = 0, NORMAL = 1, AIRBORNE = 2, ATTACKING = 3 };

struct AIComponent {
  bool on = true;
  bool movingRight = true;
  AI_STATE state = AI_STATE::NORMAL;
};

#endif
