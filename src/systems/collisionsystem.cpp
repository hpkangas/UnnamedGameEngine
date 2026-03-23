#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include <cstdio>

void CollisionSystem::update(int id, SceneInfo &currentLevel, TransformSystem &transformSystem) {

  this->transformSystem = &transformSystem;

  CollisionComponent const &cc = *currentLevel.ccs.at(id);
  TransformComponent &tc = *currentLevel.tcs.at(id);
  ControlComponent &ctrlc = *currentLevel.ctrlComponents.at(id);

  handleDownTileCollision(tc, cc, ctrlc);
  handleUpTileCollision(tc, cc, ctrlc);

  handleHorizontalBlockCollision(tc, cc);
}

void CollisionSystem::handleDownTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                                              ControlComponent &ctrlc) const {
  switch (cc.collisionTileDown) {

  case TILE::NONE:
    // ctrlc.setControlState(CONTROL_STATE::AIRBORNE);

    break;

  case TILE::SOLID:
    if (ctrlc.controlState == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    ctrlc.canClimb = true;

    break;

  case TILE::LADDERSTART:
    if (ctrlc.controlState == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
    }
    ctrlc.canClimb = true;

    break;

  default:
    break;
  }
}

void CollisionSystem::handleUpTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                                            ControlComponent &ctrlc) const {

  switch (cc.collisionTileUp) {

  case TILE::NONE:
    break;

  case TILE::SOLID:

    if (ctrlc.controlState == CONTROL_STATE::AIRBORNE) {
      if (tc.velocity.y < 0) {
        transformSystem->stopY(tc, cc.yposDif);
      }
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    ctrlc.canClimb = true;

    break;

  case TILE::LADDERSTART:
    ctrlc.canClimb = true;

    break;
  }
}

void CollisionSystem::handleHorizontalBlockCollision(TransformComponent &tc,
                                                     CollisionComponent const &cc) const {
  if (cc.collisionTileLeft == TILE::SOLID || cc.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(tc, cc.xposDif);
  }
}
