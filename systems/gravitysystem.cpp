#include "gravitysystem.hpp"

void GravitySystem::gravity(TransformComponent &transformComponent) {
  const float GRAVITY_UNIT = 0.50f;
  const float MAX_GRAVITY = 11.0;
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}

void GravitySystem::update(CollisionComponent &collisionComponent,
                           GravityComponent &gravityComponent,
                           TransformComponent &transformComponent) {
  if (collisionComponent.collisionDown) {
    gravityComponent.setGravityState(GROUNDED);
  } else {
    gravityComponent.setGravityState(INAIR);
  }

  switch (gravityComponent.getGravityState()) {
  case GROUNDED:
    break;

  case INAIR:
    gravity(transformComponent);
    break;

  default:
    break;
  }
}