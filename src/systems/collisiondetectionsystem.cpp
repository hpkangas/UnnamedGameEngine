#include "collisiondetectionsystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../utilities/physics.hpp"
#include <cstdio>
#include <iostream>

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

      if (!cc.collisionLeft || vel.x > 0) {
        checkEntityLeftTileCollision(cc, *tileColl, vel, collisionTile);
      }
      if (!cc.collisionRight || vel.x < 0) {
        checkEntityRightTileCollision(cc, *tileColl, vel, collisionTile);
      }
      if (!cc.collisionDown || vel.y > 0) {
        checkEntityDownTileCollision(cc, *tileColl, vel, collisionTile);
      }
      if (!cc.collisionUp || vel.y < 0) {
        checkEntityUpTileCollision(cc, *tileColl, vel, collisionTile);
      }
    }
  }
}

void CollisionDetectionSystem::checkEntityLeftTileCollision(CollisionComponent &entityCollision,
                                                            CollisionComponent const &tileCollision,
                                                            Velocity velocity,
                                                            TILE const &collisionTile) const {

  if (Physics::collisionLeft(entityCollision.collisionRect, tileCollision.collisionRect,
                             velocity)) {
    entityCollision.xposDif = entityCollision.collisionRect.x -
                              (tileCollision.collisionRect.x + tileCollision.collisionRect.w);
    entityCollision.collisionLeft = true;

    entityCollision.collisionTileLeft = collisionTile;
  }
}

void CollisionDetectionSystem::checkEntityRightTileCollision(
    CollisionComponent &entityCollision, CollisionComponent const &tileCollision, Velocity velocity,
    TILE const &collisionTile) const {
  if (Physics::collisionRight(entityCollision.collisionRect, tileCollision.collisionRect,
                              velocity)) {
    entityCollision.xposDif = (entityCollision.collisionRect.x + entityCollision.collisionRect.w) -
                              tileCollision.collisionRect.x;
    entityCollision.collisionTileRight = collisionTile;

    entityCollision.collisionRight = true;
  }
}

void CollisionDetectionSystem::checkEntityUpTileCollision(CollisionComponent &entityCollision,
                                                          CollisionComponent const &tileCollision,
                                                          Velocity velocity,
                                                          TILE const &collisionTile) const {
  if (Physics::collisionUp(entityCollision.collisionRect, tileCollision.collisionRect, velocity)) {
    entityCollision.yposDif = (entityCollision.collisionRect.y) -
                              (tileCollision.collisionRect.y + tileCollision.collisionRect.h);
    entityCollision.collisionTileUp = collisionTile;
    entityCollision.collidedUpTilePos = {tileCollision.collisionRect.x,
                                         tileCollision.collisionRect.y};
    entityCollision.collisionUp = true;
  }
}

void CollisionDetectionSystem::checkEntityDownTileCollision(CollisionComponent &entityCollision,
                                                            CollisionComponent const &tileCollision,
                                                            Velocity velocity,
                                                            TILE const &collisionTile) const {
  if (Physics::collisionDown(entityCollision.collisionRect, tileCollision.collisionRect,
                             velocity)) {
    entityCollision.yposDif = (entityCollision.collisionRect.y + entityCollision.collisionRect.h) -
                              tileCollision.collisionRect.y;
    entityCollision.collisionTileDown = collisionTile;
    entityCollision.collidedDownTilePos = {tileCollision.collisionRect.x,
                                           tileCollision.collisionRect.y};
    entityCollision.collisionDown = true;
  }
}

void CollisionDetectionSystem::resetCollisions(CollisionComponent &collisionComponent) const {
  collisionComponent.collisionLeft = false;
  collisionComponent.collisionRight = false;
  collisionComponent.collisionUp = false;
  collisionComponent.collisionDown = false;

  collisionComponent.collisionTileDown = TILE::NONE;
  collisionComponent.collisionTileLeft = TILE::NONE;
  collisionComponent.collisionTileRight = TILE::NONE;
  collisionComponent.collisionTileUp = TILE::NONE;
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
  projectile.collided = projectileCollision.collisionDown || projectileCollision.collisionUp ||
                        projectileCollision.collisionLeft || projectileCollision.collisionRight;
}
