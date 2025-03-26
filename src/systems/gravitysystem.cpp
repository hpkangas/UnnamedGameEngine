#include "gravitysystem.hpp"

void GravitySystem::gravity(TransformComponent &transformComponent) {
  const float GRAVITY_UNIT = 0.50f;
  const float MAX_GRAVITY = 11.0;
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}

void GravitySystem::update(bool collisionDown, bool climbing, GravityComponent &gravityComponent,
                           TransformComponent &transformComponent) {

  if (collisionDown || climbing) {
    gravityComponent.setGravityState(GROUNDED);
  } else {
    gravityComponent.setGravityState(AIRBORNE);
  }

  switch (gravityComponent.getGravityState()) {
  case GROUNDED:
    break;

  case AIRBORNE:
    gravity(transformComponent);
    break;

  default:
    break;
  }
}
