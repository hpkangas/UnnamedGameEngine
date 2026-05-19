#include "collisiondetectionsystem.hpp"
#include "../utilities/physics.hpp"
#include <cstdio>

void CollisionDetectionSystem::update() {

  for (int id : currentLevel->collisionIds) {
    CollisionComponent &cc = currentLevel->collisionComponents.at(id);
    resetCollisions(cc);
    updateCollisionRect(cc, currentLevel->transformComponents.at(id));

    checkCollision(id);
  }
}

void CollisionDetectionSystem::checkCollision(int playerIndex) const {
  CollisionComponent &object = currentLevel->collisionComponents.at(playerIndex);
  const Velocity velocity = currentLevel->transformComponents.at(playerIndex).velocity;

  // Section 2.5: instead of scanning every static collider in the level,
  // visit only the tile-grid cells overlapping a small padded bounding
  // box around the entity. With 128px tiles and per-frame speeds well
  // under 32px, a one-tile padding either side is plenty.
  TileGrid const &grid = currentLevel->staticTileGrid;
  constexpr int pad = 32;
  const int minTx = (object.collisionRect.x - pad) / 128;
  const int maxTx = (object.collisionRect.x + object.collisionRect.w + pad) / 128;
  const int minTy = (object.collisionRect.y - pad) / 128;
  const int maxTy = (object.collisionRect.y + object.collisionRect.h + pad) / 128;

  for (int ty = minTy; ty <= maxTy; ++ty) {
    for (int tx = minTx; tx <= maxTx; ++tx) {
      int id = grid.at(tx, ty);
      if (id == TileGrid::EMPTY) continue;

      CollisionComponent const *tileColl = currentLevel->collisionComponents.get(id);
      TileComponent      const *tileTile = currentLevel->tileComponents.get(id);
      if (!tileColl || !tileTile) continue;
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
  if (player.collisionRect.x + velocity.x <= 0) {
    player.xposDif = player.collisionRect.x;
    player.collisionTileLeft = TILE::SOLID;
    player.collisionLeft = true;
    return;
  }

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
  if (player.collisionRect.x + player.collisionRect.w + velocity.x >= currentLevel->levelLength) {
    player.xposDif = player.collisionRect.x + player.collisionRect.w - currentLevel->levelLength;
    player.collisionTileRight = TILE::SOLID;
    player.collisionRight = true;
    return;
  }

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
  if (player.collisionRect.y + velocity.y <= 0) {
    player.yposDif = player.collisionRect.y;
    player.collisionTileUp = TILE::SOLID;
    player.collidedUpTilePos = {0, 0};
    player.collisionUp = true;
    return;
  }

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
  if (player.collisionRect.y + player.collisionRect.h + velocity.y >= currentLevel->levelHeight) {
    player.yposDif = (player.collisionRect.y + player.collisionRect.h) - currentLevel->levelHeight;
    player.collisionTileDown = TILE::SOLID;
    player.collidedDownTilePos = {0, 0};
    player.collisionDown = true;
    return;
  }

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
