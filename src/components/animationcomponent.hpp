#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

struct AnimationFrame {
  int spriteSheetX = 0;
  int maxFrames = 0;
};

enum class ANIMATION_STATE {
  STILL = 0,
  WALKING = 1,
  PUSHING = 2,
  AIRBORNE = 3,
  CLIMBING = 4,
  ATTACKING = 5,
  SHOOTING = 6,
  DEATH = 7
};

struct AnimationComponent {
  ANIMATION_STATE animationState = ANIMATION_STATE::STILL;
  ANIMATION_STATE previousAnimationState = ANIMATION_STATE::STILL;

  int animationFrame = 0;

  AnimationFrame stillFrame {0,0};
  AnimationFrame walkingFrame {0,0};
  AnimationFrame pushingFrame {0,0};
  AnimationFrame inAirFrame {0,0};
  AnimationFrame climbingFrame {0,0};
  AnimationFrame attackingFrame {0,0};
  AnimationFrame shootingFrame {0,0};
  AnimationFrame dyingFrame {0,0};
};

#endif
