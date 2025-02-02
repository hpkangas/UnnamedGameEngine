#include "../constants.hpp"

struct AnimationComponent {
  AnimationState animationState = STILL;
  int animationFrame = 0;
  AnimationState getAnimationState() const { return animationState; }

  void setAnimationState(const AnimationState state) { animationState = state; }
};