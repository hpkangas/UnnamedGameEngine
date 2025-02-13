#include "gravitysystem.hpp"

void GravitySystem::gravity(TransformComponent &transformComponent) {
  const float GRAVITY_UNIT = 0.50f;
  const float MAX_GRAVITY = 11.0;
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}

void GravitySystem::update(Entity &selectedPlayer, CollisionComponent &collisionComponent,
                           GravityComponent &gravityComponent,
                           AnimationComponent &animationComponent,
                           TransformComponent &transformComponent) {
  if (collisionComponent.collisionDown) {
    selectedPlayer.gravityComponent.setGravityState(GROUNDED);
  } else {
    selectedPlayer.gravityComponent.setGravityState(INAIR);
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