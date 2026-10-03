#include "collisionsystem.hpp"
#include "../components/healthcomponent.hpp"
#include <cstdio>

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

  for (auto entity2 : reg.view<CollisionComponent>()) {
    CollisionComponent const &cc2 = reg.get<CollisionComponent>(entity2);
    for (auto collisionId : cc2.collisionIds) {
      if (entity2 == collisionId) {
        continue;
      }
      if (reg.any_of<ProjectileComponent>(collisionId)) {
        auto &projectile = reg.get<ProjectileComponent>(collisionId);
        if (projectile.shooterEntity == entity2) {
          continue;
        }
        if (reg.any_of<HealthComponent>(entity2)) {
          auto &health = reg.get<HealthComponent>(entity2);
          if (health.healthState != HEALTH_STATE::DEAD) {

            health.damageTaken = projectile.damage;
            projectile.collided = true;
          }
        }
      }
    }
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

void CollisionSystem::handleDownTileCollision(VelocityComponent &velocityComponent,
                                              PositionComponent &positionComponent,
                                              CollisionComponent const &cc,
                                              GravityComponent const &gc) const {
  switch (cc.collisionTileTypeDown) {

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

void CollisionSystem::handleUpTileCollision(VelocityComponent &velocityComponent,
                                            PositionComponent &positionComponent,
                                            CollisionComponent const &cc,
                                            GravityComponent const &gc) const {

  switch (cc.collisionTileTypeUp) {

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
  if (cc.collisionTileTypeLeft == TILE::SOLID || cc.collisionTileTypeRight == TILE::SOLID) {
    transformSystem->stopX(velocityComponent, positionComponent, cc.xposDif);
  }
}

void CollisionSystem::handleProjectileCollision(ProjectileComponent const &pc,
                                                entt::entity entity) const {
  if (pc.collided) {
    currentLevel->registry.destroy(entity);
  }
}
