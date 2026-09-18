#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

struct AnimationFrame {
  int spriteSheetY = 0;
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

  AnimationFrame still {0,0};
  AnimationFrame walking {0,0};
  AnimationFrame pushing {0,0};
  AnimationFrame airborne {0,0};
  AnimationFrame climbing {0,0};
  AnimationFrame attacking {0,0};
  AnimationFrame shooting {0,0};
  AnimationFrame dying {0,0};
};

#endif
