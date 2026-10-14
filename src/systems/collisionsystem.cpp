#include "collisionsystem.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/aicomponent.hpp"
#include <cstdio>

void CollisionSystem::update() {
  handleTileCollisions();
  handleProjectileCollisions();
}

void CollisionSystem::handleTileCollisions() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<CollisionComponent, PositionComponent, VelocityComponent>();
  for (auto entity : view) {
    CollisionComponent const &cc = view.get<CollisionComponent>(entity);
    VelocityComponent &velocityComponent = view.get<VelocityComponent>(entity);
    PositionComponent &positionComponent = view.get<PositionComponent>(entity);
    handleHorizontalBlockCollision(velocityComponent, positionComponent, cc);

    GravityComponent *gc = reg.try_get<GravityComponent>(entity);
    if (gc) {
      handleDownTileCollision(velocityComponent, positionComponent, cc, *gc);
      handleUpTileCollision(velocityComponent, positionComponent, cc, *gc);
    }
  }
}

void CollisionSystem::handleDownTileCollision(VelocityComponent &velocityComponent,
                                              PositionComponent &positionComponent,
                                              CollisionComponent const &cc,
                                              GravityComponent const &gc) const {
  switch (cc.collisionTileTypeDown) {

  case TILE::EMPTY:
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

  case TILE::EMPTY:
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
  case TILE::NONE:
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

void CollisionSystem::handleProjectileCollisions() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<ProjectileComponent, CollisionComponent, HealthComponent, PositionComponent, VelocityComponent>();
  for (auto projectile : reg.view<ProjectileComponent>()) {
    ProjectileComponent &pc = reg.get<ProjectileComponent>(projectile);
    CollisionComponent const &cc = reg.get<CollisionComponent>(projectile);
    HealthComponent &hc = reg.get<HealthComponent>(projectile);
    PositionComponent &posc = reg.get<PositionComponent>(projectile);
    VelocityComponent &vc = reg.get<VelocityComponent>(projectile);

    bool collided =
        cc.tileCollisionDown || cc.tileCollisionUp || cc.tileCollisionLeft || cc.tileCollisionRight;

    if (collided) {
      hc.damageTaken = pc.damage;
    }

    for (auto collidedEntity : cc.collisionIds) {
      if (collidedEntity == pc.shooterEntity) {
        continue;
      }
      if (reg.any_of<HealthComponent>(collidedEntity) && reg.any_of<AIComponent>(collidedEntity)) {
        auto &health = reg.get<HealthComponent>(collidedEntity);
        if (health.healthState != HEALTH_STATE::DEAD) {
          health.damageTaken = pc.damage;
          hc.damageTaken = pc.damage;

          //move projectile far away. Or should it be destroyed immediately?
          posc.position = {-1000,-1000};
          vc.velocity = {0, 0};
        }
      }
    }

  }
}

