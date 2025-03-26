#include "transformsystem.hpp"
#include <iostream>

void TransformSystem::moveRight(TransformComponent &transformComponent) {
  transformComponent.setXvel(transformComponent.getXvel() + playerSpeed);
  if (transformComponent.getXvel() > playerSpeedMax) {
    transformComponent.setXvel(playerSpeedMax);
  }
}

void TransformSystem::moveLeft(TransformComponent &transformComponent) {
  transformComponent.setXvel(transformComponent.getXvel() - playerSpeed);
  if (transformComponent.getXvel() < -playerSpeedMax) {
    transformComponent.setXvel(-playerSpeedMax);
  }
}

void TransformSystem::moveUp(TransformComponent &transformComponent) {
  std::cout << "moving up \n";
  transformComponent.setYvel(transformComponent.getYvel() - playerSpeed);
  if (transformComponent.getYvel() < -playerClimbingSpeedMax) {
    transformComponent.setYvel(-playerClimbingSpeedMax);
  }
}

// debugging only mostly
void TransformSystem::moveDown(TransformComponent &transformComponent) {
  transformComponent.setYvel(transformComponent.getYvel() + playerSpeed);
  if (transformComponent.getYvel() > playerClimbingSpeedMax) {
    transformComponent.setYvel(playerClimbingSpeedMax);
  }
}

// debugging only mostly
void TransformSystem::stopX(TransformComponent &transformComponent) {
  if (transformComponent.getXvel() != 0) {
    transformComponent.setXvel(0);
  }
}

void TransformSystem::stopY(TransformComponent &transformComponent) {
  if (transformComponent.getYvel() != 0) {
    transformComponent.setYvel(0);
  }
}

void TransformSystem::update(TransformComponent &transformComponent) {
  const float xvel = transformComponent.getXvel();
  const float yvel = transformComponent.getYvel();
  const int xpos = transformComponent.getXpos();
  const int ypos = transformComponent.getYpos();

  if (!(xvel == 0)) {
    transformComponent.setXpos(xpos + xvel);
  }

  if (!(yvel == 0)) {
    transformComponent.setYpos(ypos + yvel);
  }
}
