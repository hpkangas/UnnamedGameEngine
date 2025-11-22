#include "gravitysystem.hpp"

constexpr float GRAVITY_UNIT = 0.50f;
constexpr float MAX_GRAVITY = 11.0;

void GravitySystem::gravity(TransformComponent &transformComponent) {
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}

void GravitySystem::update(ControlComponent &controlComponent, GravityComponent &gravityComponent,
                           TransformComponent &transformComponent) {

  if (controlComponent.getControlState() == CONTROL_STATE::NORMAL ||
      controlComponent.getControlState() == CONTROL_STATE::CLIMBING) {
    gravityComponent.setGravityState(GRAVITY_STATE::GROUNDED);
  }

  if (controlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
    gravityComponent.setGravityState(GRAVITY_STATE::AIRBORNE);
  }

  switch (gravityComponent.getGravityState()) {
  case GRAVITY_STATE::GROUNDED:
    break;

  case GRAVITY_STATE::AIRBORNE:
    gravity(transformComponent);
    break;

  default:
    break;
  }
}
