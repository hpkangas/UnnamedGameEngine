#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include <cstdio>

void CollisionSystem::update(SceneInfo &currentLevel, TransformSystem &transformSystem) {

  this->transformSystem = &transformSystem;

  for (int id : currentLevel.collisionIds) {

    CollisionComponent const &cc = *currentLevel.collisionComponents.at(id);
    TransformComponent &tc = *currentLevel.transformsComponents.at(id);
    GravityComponent const &gc = *currentLevel.gravityComponents.at(id);

    handleDownTileCollision(tc, cc, gc);
    handleUpTileCollision(tc, cc, gc);

    handleHorizontalBlockCollision(tc, cc);
  }
}

void CollisionSystem::handleDownTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                                              GravityComponent const &gc) const {
  switch (cc.collisionTileDown) {

  case TILE::NONE:
    // ctrlc.setControlState(CONTROL_STATE::AIRBORNE);

    break;

  case TILE::SOLID:
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    // gc.canClimb = true;

    break;

  case TILE::LADDERSTART:
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
    }
    // gc.canClimb = true;

    break;

  default:
    break;
  }
}

void CollisionSystem::handleUpTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                                            GravityComponent const &gc) const {

  switch (cc.collisionTileUp) {

  case TILE::NONE:
    break;

  case TILE::SOLID:

    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      if (tc.velocity.y < 0) {
        transformSystem->stopY(tc, cc.yposDif);
      }
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    // gc.canClimb = true;

    break;

  case TILE::LADDERSTART:
    // gc.canClimb = true;

    break;
  }
}

void CollisionSystem::handleHorizontalBlockCollision(TransformComponent &tc,
                                                     CollisionComponent const &cc) const {
  if (cc.collisionTileLeft == TILE::SOLID || cc.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(tc, cc.xposDif);
  }
}
