#include "collisionsystem.hpp"
#include "../components/aicomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "transformsystem.hpp"
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
  if (cc.collidedDownTileTypes.hasTileType(TILE::SOLID)) {

    if (gc.gravityState == GRAVITY_STATE::AIRBORNE ||
        velocityComponent.velocity.y > 0 && gc.gravityState == GRAVITY_STATE::GROUNDED) {
      stopY(velocityComponent, positionComponent, cc.yposDif);
    }
  }

  if (cc.collidedDownTileTypes.hasTileType(TILE::LADDERSTART)) {
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      stopY(velocityComponent, positionComponent, cc.yposDif);
    }
  }
}

void CollisionSystem::handleUpTileCollision(VelocityComponent &velocityComponent,
                                            PositionComponent &positionComponent,
                                            CollisionComponent const &cc,
                                            GravityComponent const &gc) const {

  if (cc.collidedUpTileTypes.hasTileType(TILE::SOLID)) {
    if (gc.gravityState == GRAVITY_STATE::AIRBORNE) {
      if (velocityComponent.velocity.y < 0) {
        stopY(velocityComponent, positionComponent, cc.yposDif);
      }
    }
  }
}

void CollisionSystem::handleHorizontalBlockCollision(VelocityComponent &velocityComponent,
                                                     PositionComponent &positionComponent,
                                                     CollisionComponent const &cc) const {
  if (cc.collidedLeftTileTypes.hasTileType(TILE::SOLID) ||
      cc.collidedRightTileTypes.hasTileType(TILE::SOLID) ||
      cc.collidedLeftTileTypes.hasTileType(TILE::LADDERSTART) ||
      cc.collidedRightTileTypes.hasTileType(TILE::LADDERSTART)) {
    stopX(velocityComponent, positionComponent, cc.xposDif);
  }
}

void CollisionSystem::handleProjectileCollisions() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<ProjectileComponent, CollisionComponent, HealthComponent, PositionComponent,
                       VelocityComponent>();
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

          // move projectile far away. Or should it be destroyed immediately?
          posc.position = {-1000, -1000};
          vc.velocity = {0, 0};
        }
      }
    }
  }
}
