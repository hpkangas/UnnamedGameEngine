#include "transformsystem.hpp"
#include "../utilities/constants.hpp"

void TransformSystem::updateAll() const {
  auto view = currentLevel->registry.view<PositionComponent, VelocityComponent>();
  for (auto entity : view) {
    VelocityComponent &velocityComponent = view.get<VelocityComponent>(entity);
    PositionComponent &positionComponent = view.get<PositionComponent>(entity);
    update(velocityComponent, positionComponent);
    if (currentLevel->registry.try_get<TileComponent>(entity) &&
        currentLevel->registry.try_get<CollisionComponent>(entity)) {
      CollisionComponent &collisionComponent =
          currentLevel->registry.get<CollisionComponent>(entity);
      updateTileGridPosition(collisionComponent, entity);
    }
  }
}

void TransformSystem::update(VelocityComponent &velocityComponent,
                             PositionComponent &positionComponent) const {

  const float xvel = velocityComponent.velocity.x;
  const float yvel = velocityComponent.velocity.y;
  const int xpos = positionComponent.position.x;
  const int ypos = positionComponent.position.y;

  if (!(xvel == 0)) {
    positionComponent.position.x = xpos + xvel;
    positionComponent.tilePosition.x = positionComponent.position.x / Constants::TILE_SIZE;
  }

  if (!(yvel == 0)) {
    positionComponent.position.y = ypos + yvel;
    positionComponent.tilePosition.y = positionComponent.position.y / Constants::TILE_SIZE;
  }
}

void TransformSystem::updateTileGridPosition(CollisionComponent &collisionComponent,
                                             entt::entity entity) const {
  auto &grid = currentLevel->staticTileGrid;
  int x = collisionComponent.collisionRect.x / Constants::TILE_SIZE;
  int y = collisionComponent.collisionRect.y / Constants::TILE_SIZE;
  int topRight = (collisionComponent.collisionRect.x + collisionComponent.collisionRect.w) /
                 Constants::TILE_SIZE;
  int bottomLeft = (collisionComponent.collisionRect.y + collisionComponent.collisionRect.h) /
                   Constants::TILE_SIZE;

  std::vector<Position> positions = {Position{x, y}, Position{x, bottomLeft}, Position{topRight, y},
                                     Position{topRight, bottomLeft}};
  grid.remove(entity);
  grid.addTiles(positions, entity);
  grid.getCells(entity);
}

void moveRight(VelocityComponent &velocityComponent, PositionComponent &positionComponent) {
  positionComponent.facingRight = true;
  velocityComponent.isMoving = true;

  if (velocityComponent.velocity.x + playerSpeed > playerSpeedMax) {
    velocityComponent.velocity.x = playerSpeedMax;
  } else {
    velocityComponent.velocity.x += playerSpeed;
  }
}

void moveLeft(VelocityComponent &velocityComponent, PositionComponent &positionComponent) {
  positionComponent.facingRight = false;
  velocityComponent.isMoving = true;

  if (velocityComponent.velocity.x - playerSpeed < -playerSpeedMax) {
    velocityComponent.velocity.x = -playerSpeedMax;
  } else {
    velocityComponent.velocity.x -= playerSpeed;
  }
}

void moveUp(VelocityComponent &velocityComponent) {
  velocityComponent.isMoving = true;
  velocityComponent.velocity.y = velocityComponent.velocity.y - 8;
  if (velocityComponent.velocity.y < -playerClimbingSpeedMax) {
    velocityComponent.velocity.y = -playerClimbingSpeedMax;
  }
}

void moveDown(VelocityComponent &velocityComponent) {
  velocityComponent.isMoving = true;
  velocityComponent.velocity.y = velocityComponent.velocity.y + 8;
  if (velocityComponent.velocity.y > playerClimbingSpeedMax) {
    velocityComponent.velocity.y = playerClimbingSpeedMax;
  }
}

void jump(VelocityComponent &velocityComponent) {
  velocityComponent.isMoving = true;
  velocityComponent.velocity.y = 0;
  velocityComponent.velocity.y += playerJumpSpeed;
}

void resetJump(VelocityComponent &velocityComponent) {}

void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent) {
  stopX(velocityComponent, positionComponent, 0);
}

void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent,
           int xposDif) {

  if (velocityComponent.velocity.x != 0) {
    if (xposDif != 0) {
      positionComponent.position.x = positionComponent.position.x - xposDif;
    }
    velocityComponent.velocity.x = 0;
  }
}

void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent) {
  stopY(velocityComponent, positionComponent, 0);
}

void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent,
           int yposDif) {
  if (yposDif != 0) {
    positionComponent.position.y = positionComponent.position.y - yposDif;
  }
  velocityComponent.velocity.y = 0;
}
