#include "collisionsystem.hpp"
#include <cstdio>
#include <iostream>

void CollisionSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<GravityComponent, CollisionComponent, PositionComponent>();
  for (auto entity : view) {
    CollisionComponent const &cc = view.get<CollisionComponent>(entity);
    VelocityComponent &velocityComponent = reg.get<VelocityComponent>(entity);
    PositionComponent &positionComponent = view.get<PositionComponent>(entity);
    GravityComponent const &gc = view.get<GravityComponent>(entity);

    handleDownTileCollision(velocityComponent, positionComponent, cc, gc);
    handleUpTileCollision(velocityComponent, positionComponent, cc, gc);

    handleHorizontalBlockCollision(velocityComponent, positionComponent, cc);
  }

  auto projectiles = reg.view<ProjectileComponent>();

  for (auto entity : projectiles) {
    ProjectileComponent const &pc = projectiles.get<ProjectileComponent>(entity);
    CollisionComponent const &cc = reg.get<CollisionComponent>(entity);
    PositionComponent &positionComponent = reg.get<PositionComponent>(entity);
    VelocityComponent &velocityComponent = reg.get<VelocityComponent>(entity);

    handleHorizontalBlockCollision(velocityComponent, positionComponent, cc);
    handleProjectileCollision(pc, entity);
  }
}

void CollisionSystem::handleDownTileCollision(VelocityComponent &velocityComponent, PositionComponent &positionComponent, CollisionComponent const &cc,
                                              GravityComponent const &gc) const {
  switch (cc.collisionTileDown) {

  case TILE::NONE:
    break;

  case TILE::SOLID:
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      transformSystem->stopY(velocityComponent, positionComponent, cc.yposDif);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:
    break;

  case TILE::LADDERSTART:
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      transformSystem->stopY(velocityComponent, positionComponent, cc.yposDif);
    }
    break;

  default:
    break;
  }
}

void CollisionSystem::handleUpTileCollision(VelocityComponent &velocityComponent, PositionComponent &positionComponent, CollisionComponent const &cc,
                                            GravityComponent const &gc) const {

  switch (cc.collisionTileUp) {

  case TILE::NONE:
    break;

  case TILE::SOLID:

    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      if (velocityComponent.velocity.y < 0) {
        transformSystem->stopY(velocityComponent, positionComponent, cc.yposDif);
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

void CollisionSystem::handleHorizontalBlockCollision(VelocityComponent &velocityComponent,
                                                     PositionComponent &positionComponent,
                                                     CollisionComponent const &cc) const {
  if (cc.collisionTileLeft == TILE::SOLID || cc.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(velocityComponent, positionComponent, cc.xposDif);
  }
}

void CollisionSystem::handleProjectileCollision(ProjectileComponent const &pc,
                                                entt::entity entity) const {
  if (pc.collided) {
    currentLevel->registry.destroy(entity);
    std::cout << "Destroyed projectile entity: " << static_cast<uint32_t>(entity) << std::endl;
  }
}
