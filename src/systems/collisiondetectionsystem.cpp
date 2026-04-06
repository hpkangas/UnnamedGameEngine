#include "collisiondetectionsystem.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"
#include <cstdio>

using namespace Constants;

void CollisionDetectionSystem::update(SceneInfo &currentLevel) {
  // constructor?
  levelHeight = currentLevel.levelHeight;
  levelLength = currentLevel.levelLength;

  for (int id : currentLevel.collisionIds) {
    CollisionComponent &cc = *currentLevel.collisionComponents.at(id);
    resetCollisions(cc);
    updateCollisionRect(cc, *currentLevel.transformsComponents.at(id));
    checkCollision(id, currentLevel);
  }
}

void CollisionDetectionSystem::checkCollision(int playerIndex, SceneInfo &currentLevel) const {
  CollisionComponent &object = *currentLevel.collisionComponents.at(playerIndex);
  const Velocity velocity = currentLevel.transformsComponents.at(playerIndex).get()->velocity;

  for (int id : currentLevel.staticCollisionIds) {

    auto const &tile = currentLevel.collisionComponents.at(id);
    TILE const &collisionTile = currentLevel.tileComponents.at(id)->tile;

    // jos kollisioi useamman tilen kanssa, niin kollisioi vaan listassa ensimmäisen kanssa
    // onko kentällä reunat?

    if (!object.collisionLeft) {
      checkPlayerLeftCollision(object, *tile, velocity, collisionTile);
    }
    if (!object.collisionRight) {
      checkPlayerRightCollision(object, *tile, velocity, collisionTile);
    }
    if (!object.collisionDown) {
      checkPlayerDownCollision(object, *tile, velocity, collisionTile);
    }
    if (!object.collisionUp) {
      checkPlayerUpCollision(object, *tile, velocity, collisionTile);
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

  if (Physics::playerCollisionLeft(player.collisionRect, block.collisionRect, velocity)) {
    player.xposDif = player.collisionRect.x - (block.collisionRect.x + block.collisionRect.w);
    player.collisionTileLeft = collisionTile;

    player.collisionLeft = true;
  }
}

void CollisionDetectionSystem::checkPlayerRightCollision(CollisionComponent &player,
                                                         CollisionComponent const &block,
                                                         Velocity velocity,
                                                         TILE const &collisionTile) const {
  if (player.collisionRect.x + player.collisionRect.w + velocity.x >= levelLength) {
    player.xposDif = player.collisionRect.x + player.collisionRect.w - levelLength;
    player.collisionTileRight = TILE::SOLID;
    player.collisionRight = true;
    return;
  }

  if (Physics::playerCollisionRight(player.collisionRect, block.collisionRect, velocity)) {
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

  if (Physics::playerCollisionUp(player.collisionRect, block.collisionRect, velocity)) {
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
  if (player.collisionRect.y + player.collisionRect.h + velocity.y >= levelHeight) {
    player.yposDif = (player.collisionRect.y + player.collisionRect.h) - levelHeight;
    player.collisionTileDown = TILE::SOLID;
    player.collidedDownTilePos = {0, 0};
    player.collisionDown = true;
    return;
  }

  if (Physics::playerCollisionDown(player.collisionRect, block.collisionRect, velocity)) {
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
