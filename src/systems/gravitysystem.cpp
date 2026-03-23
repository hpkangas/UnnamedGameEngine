#include "gravitysystem.hpp"

constexpr float GRAVITY_UNIT = 0.75f;
constexpr float MAX_GRAVITY = 11.0;

void GravitySystem::gravity(TransformComponent &transformComponent) const {
  if (transformComponent.velocity.y < MAX_GRAVITY) {
    transformComponent.velocity.y = transformComponent.velocity.y + GRAVITY_UNIT;
  }
}

void GravitySystem::update(ControlComponent const &controlComponent,
                           GravityComponent &gravityComponent,
                           TransformComponent &transformComponent) const {

  if (controlComponent.controlState == CONTROL_STATE::NORMAL ||
      controlComponent.controlState == CONTROL_STATE::CLIMBING) {
    gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
  }

  if (controlComponent.controlState == CONTROL_STATE::AIRBORNE) {
    gravityComponent.gravityState = GRAVITY_STATE::AIRBORNE;
  }

  switch (gravityComponent.gravityState) {
  case GRAVITY_STATE::GROUNDED:
    break;

  case GRAVITY_STATE::AIRBORNE:
    gravity(transformComponent);
    break;

  default:
    break;
  }
}
