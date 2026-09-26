#include "collisiondetectionsystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../utilities/physics.hpp"
#include <cstdio>
#include <iostream>

void CollisionDetectionSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<CollisionComponent, TransformComponent>();
  for (auto entity : view) {
    CollisionComponent &cc = view.get<CollisionComponent>(entity);
    TransformComponent const &tc = view.get<TransformComponent>(entity);
    resetCollisions(cc);
    updateCollisionRect(cc, tc);
    checkTileCollision(entity);

    if (reg.any_of<ProjectileComponent>(entity)) {
      ProjectileComponent &pc = reg.get<ProjectileComponent>(entity);
      checkProjectileCollision(pc, cc);
    }

  }
}

void CollisionDetectionSystem::checkTileCollision(entt::entity entity) const {
  auto &reg = currentLevel->registry;
  CollisionComponent &object = reg.get<CollisionComponent>(entity);
  const Velocity velocity = reg.get<TransformComponent>(entity).velocity;

  TileGrid const &grid = currentLevel->staticTileGrid;
  constexpr int pad = 32;
  const int minTx = (object.collisionRect.x - pad) / 128;
  const int maxTx = (object.collisionRect.x + object.collisionRect.w + pad) / 128;
  const int minTy = (object.collisionRect.y - pad) / 128;
  const int maxTy = (object.collisionRect.y + object.collisionRect.h + pad) / 128;

  for (int ty = minTy; ty <= maxTy; ++ty) {
    for (int tx = minTx; tx <= maxTx; ++tx) {
      entt::entity entityAtCoordinate = grid.at(tx, ty);
      if (entityAtCoordinate == entt::null) {
        continue;
      }

      CollisionComponent const *tileColl = reg.try_get<CollisionComponent>(entityAtCoordinate);
      TileComponent const *tileTile = reg.try_get<TileComponent>(entityAtCoordinate);
      if (!tileColl || !tileTile) {
        continue;
      }
      TILE const &collisionTile = tileTile->tile;

      if (!object.collisionLeft || velocity.x > 0) {
        checkPlayerLeftCollision(object, *tileColl, velocity, collisionTile);
      }
      if (!object.collisionRight || velocity.x < 0) {
        checkPlayerRightCollision(object, *tileColl, velocity, collisionTile);
      }
      if (!object.collisionDown || velocity.y > 0) {
        checkPlayerDownCollision(object, *tileColl, velocity, collisionTile);
      }
      if (!object.collisionUp || velocity.y < 0) {
        checkPlayerUpCollision(object, *tileColl, velocity, collisionTile);
      }
    }
  }
}

void CollisionDetectionSystem::checkPlayerLeftCollision(CollisionComponent &player,
                                                        CollisionComponent const &block,
                                                        Velocity velocity,
                                                        TILE const &collisionTile) const {

  if (Physics::collisionLeft(player.collisionRect, block.collisionRect, velocity)) {
    player.xposDif = player.collisionRect.x - (block.collisionRect.x + block.collisionRect.w);
    player.collisionTileLeft = collisionTile;

    player.collisionLeft = true;
  }
}

void CollisionDetectionSystem::checkPlayerRightCollision(CollisionComponent &player,
                                                         CollisionComponent const &block,
                                                         Velocity velocity,
                                                         TILE const &collisionTile) const {
  if (Physics::collisionRight(player.collisionRect, block.collisionRect, velocity)) {
    player.xposDif = (player.collisionRect.x + player.collisionRect.w) - block.collisionRect.x;
    player.collisionTileRight = collisionTile;

    player.collisionRight = true;
  }
}

void CollisionDetectionSystem::checkPlayerUpCollision(CollisionComponent &player,
                                                      CollisionComponent const &block,
                                                      Velocity velocity,
                                                      TILE const &collisionTile) const {
  if (Physics::collisionUp(player.collisionRect, block.collisionRect, velocity)) {
    player.yposDif = (player.collisionRect.y) - (block.collisionRect.y + block.collisionRect.h);
    player.collisionTileUp = collisionTile;
    player.collidedUpTilePos = {block.collisionRect.x, block.collisionRect.y};
    player.collisionUp = true;
  }
}

void CollisionDetectionSystem::checkPlayerDownCollision(CollisionComponent &player,
                                                        CollisionComponent const &block,
                                                        Velocity velocity,
                                                        TILE const &collisionTile) const {
  if (Physics::collisionDown(player.collisionRect, block.collisionRect, velocity)) {
    player.yposDif = (player.collisionRect.y + player.collisionRect.h) - block.collisionRect.y;
    player.collisionTileDown = collisionTile;
    player.collidedDownTilePos = {block.collisionRect.x, block.collisionRect.y};
    player.collisionDown = true;
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
    CollisionComponent &collisionComponent, TransformComponent const &transformComponent) const {
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


void CollisionDetectionSystem::checkProjectileCollision(ProjectileComponent &projectile,
                                                   CollisionComponent const &projectileCollision) const {
  projectile.collided = projectileCollision.collisionDown || projectileCollision.collisionUp ||
                        projectileCollision.collisionLeft || projectileCollision.collisionRight;
}
