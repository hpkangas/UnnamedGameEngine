#include "transformsystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../utilities/constants.hpp"

void TransformSystem::updateAll() const {
  auto &reg = currentLevel->registry;
  auto view = reg.view<PositionComponent, VelocityComponent>();
  for (auto entity : view) {
    VelocityComponent &velocityComponent = view.get<VelocityComponent>(entity);
    PositionComponent &positionComponent = view.get<PositionComponent>(entity);
    const int previousX = positionComponent.position.x;
    const int previousY = positionComponent.position.y;
    update(velocityComponent, positionComponent);

    TileComponent *tileComponent = reg.try_get<TileComponent>(entity);
    if (!tileComponent ||
        (previousX == positionComponent.position.x &&
         previousY == positionComponent.position.y)) {
      continue;
    }

    TileGrid &tileGrid = currentLevel->staticTileGrid;
    CollisionComponent const *collision = reg.try_get<CollisionComponent>(entity);
    if (collision) {
      tileGrid.removeRectangle(previousX, previousY, collision->collisionRect.w,
                               collision->collisionRect.h, Constants::TILE_SIZE, entity);
      tileGrid.addRectangle(positionComponent.position.x, positionComponent.position.y,
                            collision->collisionRect.w, collision->collisionRect.h,
                            Constants::TILE_SIZE, entity);
    } else {
      tileGrid.remove(tileComponent->tilePosition.x, tileComponent->tilePosition.y, entity);
      tileGrid.add(positionComponent.tilePosition.x, positionComponent.tilePosition.y, entity);
    }

    tileComponent->tilePosition = positionComponent.tilePosition;
  }
}

void TransformSystem::update(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const {

  const float xvel = velocityComponent.velocity.x;
  const float yvel = velocityComponent.velocity.y;
  const int xpos = positionComponent.position.x;
  const int ypos = positionComponent.position.y;

  if(xvel > 0) {
    positionComponent.facingRight = true;
  } else if (xvel < 0) {
    positionComponent.facingRight = false;
  }

  if (!(xvel == 0)) {
    positionComponent.position.x = xpos + xvel;
    positionComponent.tilePosition.x = positionComponent.position.x / Constants::TILE_SIZE;
  }

  if (!(yvel == 0)) {
    positionComponent.position.y = ypos + yvel;
    positionComponent.tilePosition.y = positionComponent.position.y / Constants::TILE_SIZE;
  }
}

void moveRight(VelocityComponent &velocityComponent, PositionComponent &positionComponent)  {
  positionComponent.facingRight = true;
  if (velocityComponent.velocity.x + playerSpeed > playerSpeedMax) {
    velocityComponent.velocity.x = playerSpeedMax;
  } else {
    velocityComponent.velocity.x += playerSpeed;
  }
}

void moveLeft(VelocityComponent &velocityComponent, PositionComponent &positionComponent)  {
  positionComponent.facingRight = false;
  if (velocityComponent.velocity.x - playerSpeed < -playerSpeedMax) {
    velocityComponent.velocity.x = -playerSpeedMax;
  } else {
    velocityComponent.velocity.x -= playerSpeed;
  }
}

void moveUp(VelocityComponent &velocityComponent)  {
  velocityComponent.velocity.y = velocityComponent.velocity.y - 8;
  if (velocityComponent.velocity.y < -playerClimbingSpeedMax) {
    velocityComponent.velocity.y = -playerClimbingSpeedMax;
  }
}

void moveDown(VelocityComponent &velocityComponent)  {
  velocityComponent.velocity.y = velocityComponent.velocity.y + 8;
  if (velocityComponent.velocity.y > playerClimbingSpeedMax) {
    velocityComponent.velocity.y = playerClimbingSpeedMax;
  }
}

void jump(VelocityComponent &velocityComponent)  {
  velocityComponent.velocity.y = playerJumpSpeed;
}

void resetJump(VelocityComponent &velocityComponent)  {}

void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent)  {
  stopX(velocityComponent, positionComponent, 0);
}

void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int xposDif)  {

  if (velocityComponent.velocity.x != 0) {
    if (xposDif != 0) {
      positionComponent.position.x = positionComponent.position.x - xposDif;
    }
    velocityComponent.velocity.x = 0;
  }
}

void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent)  {
  stopY(velocityComponent, positionComponent, 0);
}

void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int yposDif)  {
  if (yposDif != 0) {
    positionComponent.position.y = positionComponent.position.y - yposDif;
  }
  velocityComponent.velocity.y = 0;
}
