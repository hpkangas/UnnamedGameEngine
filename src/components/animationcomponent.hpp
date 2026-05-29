#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

enum class ANIMATION_STATE {
  STILL = 0,
  WALKING = 1,
  PUSHING = 2,
  AIRBORNE = 3,
  CLIMBING = 4,
  ATTACKING = 5,
  DEATH = 6
};

struct AnimationComponent {
  ANIMATION_STATE animationState = ANIMATION_STATE::STILL;
  ANIMATION_STATE previousAnimationState = ANIMATION_STATE::STILL;

  int animationFrame = 0;
  const int WALKING_ANIMATION_FRAMES = 8;

  int STILL_FRAME = 0;
  int WALK_FRAME = 0;
  int PUSH_FRAME = 0;
  int INAIR_FRAME = 0;
  int CLIMBING_FRAME = 0;
  int ATTACKING_FRAME = 0;
  int DYING_FRAME = 0;
};

#endif
