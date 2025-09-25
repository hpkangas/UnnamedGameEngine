#include "gravitysystem.hpp"
#include <iostream>

const float GRAVITY_UNIT = 0.50f;
const float MAX_GRAVITY = 11.0;

void GravitySystem::gravity(TransformComponent &transformComponent) {
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITY_UNIT);
  }
}

void GravitySystem::update(ControlComponent &controlComponent, GravityComponent &gravityComponent,
                           TransformComponent &transformComponent) {

  std::cout << "GRAVITY: ";

  if (controlComponent.getControlState() == CONTROL_STATE::NORMAL ||
      controlComponent.getControlState() == CONTROL_STATE::CLIMBING) {
    gravityComponent.setGravityState(GRAVITY_STATE::GROUNDED);
  }

  if (controlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
    gravityComponent.setGravityState(GRAVITY_STATE::AIRBORNE);
  }

  switch (gravityComponent.getGravityState()) {
  case GRAVITY_STATE::GROUNDED:
    std::cout << "grounded";
    break;

  case GRAVITY_STATE::AIRBORNE:
    std::cout << "gravity";
    // controlComponent.controlState = CONTROL_STATE::AIRBORNE;
    gravity(transformComponent);
    break;

  default:
    break;
  }
  std::cout << "gravity done \n";
}
