#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP
#include "component.hpp"

enum class ANIMATION_STATE { STILL = 0, WALKING = 1, PUSHING = 2, AIRBORNE = 3, CLIMBING = 4 };

struct AnimationComponent : public IComponent {

  ANIMATION_STATE animationState = ANIMATION_STATE::STILL;
  int animationFrame = 0;
  const int WALKING_ANIMATION_FRAMES = 8;
};

#endif
