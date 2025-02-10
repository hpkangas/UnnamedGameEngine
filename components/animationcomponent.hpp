enum AnimationState { STILL = 0, WALKING = 1, PUSHING = 2, anims_INAIR = 3 };

struct AnimationComponent {

  AnimationState animationState = STILL;
  int animationFrame = 0;
  const int WALKING_ANIMATION_FRAMES = 8;

  AnimationState getAnimationState() const { return animationState; }

  void setAnimationState(const AnimationState state) { animationState = state; }
};