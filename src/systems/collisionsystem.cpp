#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include <cstdio>

void CollisionSystem::update() {
  // Iterate the gravity pool: every entity that needs per-frame
  // collision *response* is gravity-affected in this codebase. Static
  // tiles are never resolved against the world, so we skip them
  // implicitly by driving off the smaller pool.
  auto &gravityPool = currentLevel->gravityComponents;
  for (std::size_t i = 0; i < gravityPool.size(); ++i) {
    const int id = gravityPool.entityAt(i);

    CollisionComponent const &cc = currentLevel->collisionComponents.at(id);
    TransformComponent &tc = currentLevel->transformComponents.at(id);
    GravityComponent const &gc = gravityPool[i];

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
