#include "transformsystem.hpp"

void TransformSystem::moveRight(TransformComponent &transformComponent) const {
  if (transformComponent.velocity.x + playerSpeed > playerSpeedMax) {
    transformComponent.velocity.x = playerSpeedMax;
  } else {
    transformComponent.velocity.x = transformComponent.velocity.x + playerSpeed;
  }
}

void TransformSystem::moveLeft(TransformComponent &transformComponent) const {
  if (transformComponent.velocity.x < -playerSpeedMax) {
    transformComponent.velocity.x = -playerSpeedMax;
  } else {
    transformComponent.velocity.x = transformComponent.velocity.x - playerSpeed;
  }
}

void TransformSystem::moveUp(TransformComponent &transformComponent) const {
  transformComponent.velocity.y = transformComponent.velocity.y - 8;
  if (transformComponent.velocity.y < -playerClimbingSpeedMax) {
    transformComponent.velocity.y = -playerClimbingSpeedMax;
  }
}

void TransformSystem::moveDown(TransformComponent &transformComponent) const {
  transformComponent.velocity.y = transformComponent.velocity.y + 8;
  if (transformComponent.velocity.y > playerClimbingSpeedMax) {
    transformComponent.velocity.y = playerClimbingSpeedMax;
  }
}

void TransformSystem::jump(TransformComponent &transformComponent) const {
  transformComponent.velocity.y = playerJumpSpeed;
}

void TransformSystem::resetJump(TransformComponent &transformComponent) const {}

void TransformSystem::stopX(TransformComponent &transformComponent) const {
  stopX(transformComponent, 0);
}

void TransformSystem::stopX(TransformComponent &transformComponent, int xposDif) const {

  if (transformComponent.velocity.x != 0) {
    if (xposDif != 0) {
      transformComponent.position.x = transformComponent.position.x - xposDif;
    }
    transformComponent.velocity.x = 0;
  }
}

void TransformSystem::stopY(TransformComponent &transformComponent) const {
  stopY(transformComponent, 0);
}

void TransformSystem::stopY(TransformComponent &transformComponent, int yposDif) const {
  if (yposDif != 0) {
    transformComponent.position.y = transformComponent.position.y - yposDif;
  }
  transformComponent.velocity.y = 0;
}

void TransformSystem::update(TransformComponent &transformComponent) const {

  const float xvel = transformComponent.velocity.x;
  const float yvel = transformComponent.velocity.y;
  const int xpos = transformComponent.position.x;
  const int ypos = transformComponent.position.y;

  if (!(xvel == 0)) {
    transformComponent.position.x = xpos + xvel;
  }

  if (!(yvel == 0)) {
    transformComponent.position.y = ypos + yvel;
  }
}
