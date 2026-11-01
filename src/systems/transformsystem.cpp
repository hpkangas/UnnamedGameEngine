#include "transformsystem.hpp"
#include "../utilities/constants.hpp"

void TransformSystem::updateAll() const {
  auto view = currentLevel->registry.view<PositionComponent, VelocityComponent>();
  for (auto entity : view) {
    VelocityComponent &velocityComponent = view.get<VelocityComponent>(entity);
    PositionComponent &positionComponent = view.get<PositionComponent>(entity);
    update(velocityComponent, positionComponent);
  }
}

void TransformSystem::moveRight(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const {
  positionComponent.facingRight = true;
  if (velocityComponent.velocity.x + playerSpeed > playerSpeedMax) {
    velocityComponent.velocity.x = playerSpeedMax;
  } else {
    velocityComponent.velocity.x += playerSpeed;
  }
}

void TransformSystem::moveLeft(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const {
  positionComponent.facingRight = false;
  if (velocityComponent.velocity.x - playerSpeed < -playerSpeedMax) {
    velocityComponent.velocity.x = -playerSpeedMax;
  } else {
    velocityComponent.velocity.x -= playerSpeed;
  }
}

void TransformSystem::moveUp(VelocityComponent &velocityComponent) const {
  velocityComponent.velocity.y = velocityComponent.velocity.y - 8;
  if (velocityComponent.velocity.y < -playerClimbingSpeedMax) {
    velocityComponent.velocity.y = -playerClimbingSpeedMax;
  }
}

void TransformSystem::moveDown(VelocityComponent &velocityComponent) const {
  velocityComponent.velocity.y = velocityComponent.velocity.y + 8;
  if (velocityComponent.velocity.y > playerClimbingSpeedMax) {
    velocityComponent.velocity.y = playerClimbingSpeedMax;
  }
}

void TransformSystem::jump(VelocityComponent &velocityComponent) const {
  velocityComponent.velocity.y = playerJumpSpeed;
}

void TransformSystem::resetJump(VelocityComponent &velocityComponent) const {}

void TransformSystem::stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const {
  stopX(velocityComponent, positionComponent, 0);
}

void TransformSystem::stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int xposDif) const {

  if (velocityComponent.velocity.x != 0) {
    if (xposDif != 0) {
      positionComponent.position.x = positionComponent.position.x - xposDif;
    }
    velocityComponent.velocity.x = 0;
  }
}

void TransformSystem::stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const {
  stopY(velocityComponent, positionComponent, 0);
}

void TransformSystem::stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int yposDif) const {
  if (yposDif != 0) {
    positionComponent.position.y = positionComponent.position.y - yposDif;
  }
  velocityComponent.velocity.y = 0;
}

void TransformSystem::update(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const {

  const float xvel = velocityComponent.velocity.x;
  const float yvel = velocityComponent.velocity.y;
  const int xpos = positionComponent.position.x;
  const int ypos = positionComponent.position.y;

  //TODO magic numbers
  if (!(xvel == 0)) {
    positionComponent.position.x = xpos + xvel;
    positionComponent.tilePosition.x = positionComponent.position.x / Constants::TILE_SIZE;
  }

  if (!(yvel == 0)) {
    positionComponent.position.y = ypos + yvel;
    positionComponent.tilePosition.y = positionComponent.position.y / Constants::TILE_SIZE;
  }
}
