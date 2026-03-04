#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include <cstdio>

void CollisionSystem::update(int id, SceneInfo &currentLevel, TransformSystem &transformSystem) {

  this->transformSystem = &transformSystem;

  CollisionComponent &cc = *currentLevel.ccs.at(id);
  TransformComponent &tc = *currentLevel.tcs.at(id);
  ControlComponent &ctrlc = *currentLevel.ctrlComponents.at(id);

  handleDownTileCollision(tc, cc, ctrlc);
  // handleUpTileCollision(playerT, playerC, playerControl);

  handleHorizontalBlockCollision(tc, cc);
}

void CollisionSystem::handleDownTileCollision(TransformComponent &tc, CollisionComponent &cc,
                                              ControlComponent &ctrlc) {
  switch (cc.collisionTileDown) {

  case TILE::NONE:
    // ctrlc.setControlState(CONTROL_STATE::AIRBORNE);

    break;

  case TILE::SOLID:
    if (ctrlc.getControlState() == CONTROL_STATE::AIRBORNE) {
      // I'm not sure I like collision system deciding what the control state is but for now let's
      // go with this
      transformSystem->stopY(tc, cc.yposDif);
      //removal causes pc to go through ground
      ctrlc.setControlState(CONTROL_STATE::NORMAL);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:

    break;

  case TILE::LADDERSTART:
    if (ctrlc.getControlState() == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
      // ctrlc.setControlState(CONTROL_STATE::NORMAL);
    }

    break;

  default:
    break;
  }
}

void CollisionSystem::handleUpTileCollision(TransformComponent &tc, CollisionComponent &cc,
                                            ControlComponent &ctrlc) {

  switch (cc.collisionTileUp) {

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
