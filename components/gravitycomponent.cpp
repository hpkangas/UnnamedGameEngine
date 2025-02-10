#include "gravitycomponent.hpp"
#include "transformcomponent.hpp"

void GravityComponent::gravity(TransformComponent &transformComponent) {
  const float GRAVITY_UNIT = 0.50f;
  const float MAX_GRAVITY = 11.0;
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}