#include "transformsystem.hpp"

void TransformSystem::moveRight(TransformComponent &transformComponent) {
  transformComponent.setXvel(transformComponent.getXvel() + playerSpeed);
  if (transformComponent.getXvel() > playerSpeedMax)
    transformComponent.setXvel(playerSpeedMax);
}

void TransformSystem::moveLeft(TransformComponent &transformComponent) {
  transformComponent.setXvel(transformComponent.getXvel() - playerSpeed);
  if (transformComponent.getXvel() < -playerSpeedMax)
    transformComponent.setXvel(-playerSpeedMax);
}

void TransformSystem::moveUp(TransformComponent &transformComponent) {
  transformComponent.setYvel(-playerSpeed);
}

// debugging only mostly
void TransformSystem::moveDown(TransformComponent &transformComponent) {
  transformComponent.setYvel(playerSpeed);
}

// debugging only mostly
void TransformSystem::stopX(TransformComponent &transformComponent) {
  if (transformComponent.getXvel() != 0) {
    transformComponent.setXvel(0);
  }
}

// debugging only mostly
void TransformSystem::stopYU(TransformComponent &transformComponent) {
  if (transformComponent.velocity.y < 0) {
    transformComponent.velocity.y = 0;
  }
}

// debugging only mostly
void TransformSystem::stopYD(TransformComponent &transformComponent) {
  if (transformComponent.velocity.y > 0) {
    transformComponent.velocity.y = 0;
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