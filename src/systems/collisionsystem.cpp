#include "collisionsystem.hpp"
#include "transformsystem.hpp"
#include "../components/projectilecomponent.hpp"
#include <cstdio>

void CollisionSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<GravityComponent, CollisionComponent, TransformComponent>();
  for (auto entity : view) {
    CollisionComponent const &cc = view.get<CollisionComponent>(entity);
    TransformComponent &tc = view.get<TransformComponent>(entity);
    GravityComponent const &gc = view.get<GravityComponent>(entity);

    handleDownTileCollision(tc, cc, gc);
    handleUpTileCollision(tc, cc, gc);

    handleHorizontalBlockCollision(tc, cc);
  }

  auto projectiles = reg.view<ProjectileComponent>();

   for (auto entity : projectiles) {
    ProjectileComponent const &pc = projectiles.get<ProjectileComponent>(entity);
    CollisionComponent const &cc = reg.get<CollisionComponent>(entity);
    TransformComponent &tc = reg.get<TransformComponent>(entity);

    handleHorizontalBlockCollision(tc, cc);
  }
}

void CollisionSystem::handleDownTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                                              GravityComponent const &gc) const {
  switch (cc.collisionTileDown) {

  case TILE::NONE:
    break;

  case TILE::SOLID:
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    break;

  case TILE::LADDERSTART:
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      transformSystem->stopY(tc, cc.yposDif);
    }
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
    break;

  case TILE::LADDERSTART:

    break;
  }
}

void CollisionSystem::handleHorizontalBlockCollision(TransformComponent &tc,
                                                     CollisionComponent const &cc) const {
  if (cc.collisionTileLeft == TILE::SOLID || cc.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(tc, cc.xposDif);
  }
}
