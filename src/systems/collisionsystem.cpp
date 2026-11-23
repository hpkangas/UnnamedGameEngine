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
    VelocityComponent &vc = view.get<VelocityComponent>(entity);
    PositionComponent &pc = view.get<PositionComponent>(entity);
    handleHorizontalBlockCollision(vc, pc, cc);

    handleDownTileCollision(vc, pc, cc);
    handleUpTileCollision(vc, pc, cc);
  }
}

void CollisionSystem::handleDownTileCollision(VelocityComponent &vc,
                                              PositionComponent &pc,
                                              CollisionComponent const &cc) const {

  if (cc.down.tileTypes.hasTileType(TILE::SOLID)) {
    stopY(vc, pc, cc.down.positionDifference.y);

    if (vc.isMoving) {
      vc.velocity.y += cc.down.velocity.y;
    } else {
      vc.velocity += cc.down.velocity;
    }
  }

  if (cc.down.tileTypes.hasTileType(TILE::LADDERSTART)) {
    if (!vc.isMoving) {
      //TODO Teleports out of ladderstart tile to top but requires design decisions about lerping etc
      stopY(vc, pc, cc.down.positionDifference.y);
    }
  }
}

void CollisionSystem::handleUpTileCollision(VelocityComponent &vc,
                                            PositionComponent &pc,
                                            CollisionComponent const &cc) const {

  if (cc.up.tileTypes.hasTileType(TILE::SOLID)) {
    if (vc.velocity.y < 0) {
      stopY(vc, pc, cc.up.positionDifference.y);
    }
  }
}

void CollisionSystem::handleHorizontalBlockCollision(VelocityComponent &vc,
                                                     PositionComponent &pc,
                                                     CollisionComponent const &cc) const {
  if (cc.left.tileTypes.hasTileType(TILE::SOLID) ||
      cc.left.tileTypes.hasTileType(TILE::LADDERSTART)) {
    stopX(vc, pc, cc.left.positionDifference.x);
    vc.velocity.x += cc.left.velocity.x;
  } else if (cc.right.tileTypes.hasTileType(TILE::SOLID) ||
             cc.right.tileTypes.hasTileType(TILE::LADDERSTART)) {
    stopX(vc, pc, cc.right.positionDifference.x);
    vc.velocity.x += cc.right.velocity.x;
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

    bool collided = cc.down.collision || cc.up.collision || cc.left.collision || cc.right.collision;

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
