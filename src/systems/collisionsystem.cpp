#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include <cstdio>
#include <iostream>

void CollisionSystem::update(int i, SceneInfo &currentLevel, TransformSystem &transformSystem) {
  std::cout << "COLLISION SYSTEM PLAYER " << i << "\n";

  this->transformSystem = &transformSystem;

  CollisionComponent &playerC = *currentLevel.ccs.at(i);
  TransformComponent &playerT = *currentLevel.tcs.at(i);
  ControlComponent &playerControl = *currentLevel.controlComponents.at(i);

  handleDownTileCollision(playerT, playerC, playerControl);
  handleUpTileCollision(playerT, playerC, playerControl);

  handleHorizontalBlockCollision(playerT, playerC);
}

void CollisionSystem::handleDownTileCollision(TransformComponent &playerTransformComponent,
                                              CollisionComponent &playerCollisionComponent,
                                              ControlComponent &playerControlComponent) {

  // ei kollisioi kun kiipeää
  switch (playerCollisionComponent.collisionTileDown) {

  case TILE::NONE:
    std::cout << "down none tile \n";

    // if (playerCollisionComponent.collisionTileUp == TILE::NONE) {
    //   playerControlComponent.controlState = CONTROL_STATE::AIRBORNE;
    // }

    // playerControlComponent.setCanClimbDown(false);
    // playerControlComponent.canClimbUp = false;
    break;

  case TILE::SOLID:
    std::cout << "down solid tile \n";
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
    std::cout << "down ladder tile \n";

    break;

  case TILE::LADDERSTART:
    std::cout << "down ladderstart tile \n";
    if (playerControlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(playerTransformComponent, playerCollisionComponent.yposDif);
      playerControlComponent.setControlState(CONTROL_STATE::NORMAL);
    }

    break;

  default:
    // playerCollisionComponent.collisionTileDown = TILE::NONE;
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
    std::cout << "up ladder tile \n";
    break;

  case TILE::LADDERSTART:
    std::cout << "up ladderstart tile \n";
    break;
  }
}

void CollisionSystem::handleHorizontalBlockCollision(TransformComponent &tc,
                                                     CollisionComponent &cc) {
  if (cc.collisionTileLeft == TILE::SOLID || cc.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(tc, cc.xposDif);
  }
}
