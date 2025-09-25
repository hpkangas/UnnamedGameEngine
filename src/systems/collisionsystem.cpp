#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include <cstdio>

void CollisionSystem::update(int i, SceneInfo &currentLevel, TransformSystem &transformSystem) {

  this->transformSystem = &transformSystem;

  CollisionComponent &playerC = *currentLevel.ccs.at(i);
  TransformComponent &playerT = *currentLevel.tcs.at(i);
  ControlComponent &playerControl = *currentLevel.controlComponents.at(i);

  handleDownTileCollision(playerT, playerC, playerControl);
  // handleUpTileCollision(playerT, playerC, playerControl);

  handleHorizontalBlockCollision(playerT, playerC);
}

void CollisionSystem::handleDownTileCollision(TransformComponent &playerTransformComponent,
                                              CollisionComponent &playerCollisionComponent,
                                              ControlComponent &playerControlComponent) {

  switch (playerCollisionComponent.collisionTileDown) {

  case TILE::NONE:
    playerControlComponent.setControlState(CONTROL_STATE::AIRBORNE);

    break;

  case TILE::SOLID:
    if (playerControlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
      // I'm not sure I like collision system deciding what the control state is but for now let's
      // go with this
      transformSystem->stopY(playerTransformComponent, playerCollisionComponent.yposDif);
      playerControlComponent.setControlState(CONTROL_STATE::NORMAL);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:

    break;

  case TILE::LADDERSTART:
    if (playerControlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(playerTransformComponent, playerCollisionComponent.yposDif);
      playerControlComponent.setControlState(CONTROL_STATE::NORMAL);
    }

    break;

  default:
    break;
  }
}

void CollisionSystem::handleUpTileCollision(TransformComponent &playerTransformComponent,
                                            CollisionComponent &playerCollisionComponent,
                                            ControlComponent &playerControlComponent) {

  switch (playerCollisionComponent.collisionTileUp) {

  case TILE::NONE:
    break;

  case TILE::SOLID:
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    break;

  case TILE::LADDERSTART:
    break;
  }
}

void CollisionSystem::handleHorizontalBlockCollision(TransformComponent &tc,
                                                     CollisionComponent &cc) {
  if (cc.collisionTileLeft == TILE::SOLID || cc.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(tc, cc.xposDif);
  }
}
