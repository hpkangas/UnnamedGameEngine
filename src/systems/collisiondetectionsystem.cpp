#include "collisiondetectionsystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../utilities/physics.hpp"
#include <cstdio>

void CollisionDetectionSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<CollisionComponent, PositionComponent, VelocityComponent>();
  for (auto entity : view) {
    CollisionComponent &cc = view.get<CollisionComponent>(entity);
    PositionComponent const &pc = view.get<PositionComponent>(entity);
    VelocityComponent const &vc = view.get<VelocityComponent>(entity);
    resetCollisions(cc);
    updateCollisionRect(cc, pc);

    checkTileCollision(cc, pc, vc);

    if (reg.any_of<ProjectileComponent>(entity)) {
      ProjectileComponent &pc = reg.get<ProjectileComponent>(entity);
      checkProjectileCollision(pc, cc);
    }

    for(auto entity2 : view) {
      if(entity == entity2) {
        continue;
      }
      CollisionComponent &cc2 = view.get<CollisionComponent>(entity2);
      PositionComponent const &pc2 = view.get<PositionComponent>(entity2);
      VelocityComponent const &vc2 = view.get<VelocityComponent>(entity2);

      if(Physics::checkCollision(cc.collisionRect, cc2.collisionRect)) {
        // Handle collision
        cc.collisionIds.emplace_back(entity2);
      }

    }
  }
}

void CollisionDetectionSystem::checkTileCollision(CollisionComponent &cc,
                                                  PositionComponent const &position,
                                                  VelocityComponent const &velocity) const {
  auto &reg = currentLevel->registry;
  const Velocity &vel = velocity.velocity;

  TileGrid const &grid = currentLevel->staticTileGrid;
  constexpr int pad = 32;
  const int minTx = (cc.collisionRect.x - pad) / 128;
  const int maxTx = (cc.collisionRect.x + cc.collisionRect.w + pad) / 128;
  const int minTy = (cc.collisionRect.y - pad) / 128;
  const int maxTy = (cc.collisionRect.y + cc.collisionRect.h + pad) / 128;

  for (int ty = minTy; ty <= maxTy; ++ty) {
    for (int tx = minTx; tx <= maxTx; ++tx) {
      entt::entity tileAtCoordinate = grid.at(tx, ty);
      if (tileAtCoordinate == entt::null) {
        continue;
      }

      CollisionComponent const *tileColl = reg.try_get<CollisionComponent>(tileAtCoordinate);
      TileComponent const *tileTile = reg.try_get<TileComponent>(tileAtCoordinate);
      if (!tileColl || !tileTile) {
        continue;
      }
      TILE const &collisionTile = tileTile->tile;

      if (!cc.tileCollisionLeft || vel.x > 0) {
        checkEntityLeftTileCollision(cc, *tileColl, vel, collisionTile);
      }
      if (!cc.tileCollisionRight || vel.x < 0) {
        checkEntityRightTileCollision(cc, *tileColl, vel, collisionTile);
      }
      if (!cc.tileCollisionDown || vel.y > 0) {
        checkEntityDownTileCollision(cc, *tileColl, vel, collisionTile);
      }
      if (!cc.tileCollisionUp || vel.y < 0) {
        checkEntityUpTileCollision(cc, *tileColl, vel, collisionTile);
      }
    }
  }
}

void CollisionDetectionSystem::checkEntityLeftTileCollision(CollisionComponent &objectCollision,
                                                            CollisionComponent const &tileCollision,
                                                            Velocity velocity,
                                                            TILE const &collisionTile) const {

  if (Physics::collisionLeft(objectCollision.collisionRect, tileCollision.collisionRect,
                             velocity)) {
    objectCollision.xposDif = objectCollision.collisionRect.x -
                              (tileCollision.collisionRect.x + tileCollision.collisionRect.w);
    objectCollision.tileCollisionLeft = true;

    objectCollision.collisionTileTypeLeft = collisionTile;
  }
}

void CollisionDetectionSystem::checkEntityRightTileCollision(
    CollisionComponent &objectCollision, CollisionComponent const &tileCollision, Velocity velocity,
    TILE const &collisionTile) const {
  if (Physics::collisionRight(objectCollision.collisionRect, tileCollision.collisionRect,
                              velocity)) {
    objectCollision.xposDif = (objectCollision.collisionRect.x + objectCollision.collisionRect.w) -
                              tileCollision.collisionRect.x;
    objectCollision.collisionTileTypeRight = collisionTile;

    objectCollision.tileCollisionRight = true;
  }
}

void CollisionDetectionSystem::checkEntityUpTileCollision(CollisionComponent &objectCollision,
                                                          CollisionComponent const &tileCollision,
                                                          Velocity velocity,
                                                          TILE const &collisionTile) const {
  if (Physics::collisionUp(objectCollision.collisionRect, tileCollision.collisionRect, velocity)) {
    objectCollision.yposDif = (objectCollision.collisionRect.y) -
                              (tileCollision.collisionRect.y + tileCollision.collisionRect.h);
    objectCollision.collisionTileTypeUp = collisionTile;
    objectCollision.collidedUpTilePos = {tileCollision.collisionRect.x,
                                         tileCollision.collisionRect.y};
    objectCollision.tileCollisionUp = true;
  }
}

void CollisionDetectionSystem::checkEntityDownTileCollision(CollisionComponent &objectCollision,
                                                            CollisionComponent const &tileCollision,
                                                            Velocity velocity,
                                                            TILE const &collisionTile) const {
  if (Physics::collisionDown(objectCollision.collisionRect, tileCollision.collisionRect,
                             velocity)) {
    objectCollision.yposDif = (objectCollision.collisionRect.y + objectCollision.collisionRect.h) -
                              tileCollision.collisionRect.y;
    objectCollision.collisionTileTypeDown = collisionTile;
    objectCollision.collidedDownTilePos = {tileCollision.collisionRect.x,
                                           tileCollision.collisionRect.y};
    objectCollision.tileCollisionDown = true;
  }
}

void CollisionDetectionSystem::resetCollisions(CollisionComponent &collisionComponent) const {
  collisionComponent.tileCollisionLeft = false;
  collisionComponent.tileCollisionRight = false;
  collisionComponent.tileCollisionUp = false;
  collisionComponent.tileCollisionDown = false;

  collisionComponent.collisionTileTypeDown = TILE::NONE;
  collisionComponent.collisionTileTypeLeft = TILE::NONE;
  collisionComponent.collisionTileTypeRight = TILE::NONE;
  collisionComponent.collisionTileTypeUp = TILE::NONE;

  collisionComponent.collisionIds.clear();
}

void CollisionDetectionSystem::updateCollisionRect(
    CollisionComponent &collisionComponent, PositionComponent const &transformComponent) const {
  collisionComponent.collisionRect.x = transformComponent.position.x;
  collisionComponent.collisionRect.y = transformComponent.position.y;

  collisionComponent.center = {
      (collisionComponent.collisionRect.x + collisionComponent.collisionRect.w +
       collisionComponent.collisionRect.x) /
          2,
      (collisionComponent.collisionRect.y + collisionComponent.collisionRect.h +
       collisionComponent.collisionRect.y) /
          2};
}

void CollisionDetectionSystem::checkProjectileCollision(
    ProjectileComponent &projectile, CollisionComponent const &projectileCollision) const {
  projectile.collided = projectileCollision.tileCollisionDown || projectileCollision.tileCollisionUp ||
                        projectileCollision.tileCollisionLeft || projectileCollision.tileCollisionRight;
}
