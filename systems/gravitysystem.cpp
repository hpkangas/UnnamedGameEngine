#include "gravitysystem.hpp"

void GravitySystem::gravity(TransformComponent &transformComponent) {
  const float GRAVITY_UNIT = 0.50f;
  const float MAX_GRAVITY = 11.0;
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}

void GravitySystem::update(GravityComponent &gravityComponent,
                           AnimationComponent &animationComponent,
                           TransformComponent &transformComponent) {
  switch (gravityComponent.getGravityState()) {
  case GROUNDED:
    if (animationComponent.getAnimationState() == anims_INAIR)
      animationComponent.setAnimationState(STILL);
    break;

  case INAIR:
    gravity(transformComponent);
    animationComponent.setAnimationState(anims_INAIR);
    break;

  default:
    break;
  }
}