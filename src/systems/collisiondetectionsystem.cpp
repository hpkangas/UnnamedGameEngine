#include "collisiondetectionsystem.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"

#include <cstdio>

void CollisionDetectionSystem::update(int i, SceneInfo &currentLevel) const {
  checkBlockCollision(i, currentLevel);
}

void CollisionDetectionSystem::checkBlockCollision(int i, SceneInfo &currentLevel) const {
  CollisionComponent &playerC = *currentLevel.ccs.at(i);
  TransformComponent const &playerT = *currentLevel.tcs.at(i);

  updatePlayerCollisionRect(playerC, playerT);
  resetPlayerCollisions(playerC);
  Velocity velocity = playerT.velocity;
  // other than player ids
  for (int j : currentLevel.collisionIds) {
    if (j == PLAYER_INDEX) {
      continue;
    }
    auto const &blockC = currentLevel.ccs.at(j);
    TILE collisionTile = TILE::NONE;
    if (currentLevel.tiles.contains(j)) {
      collisionTile = currentLevel.tiles.at(j)->tile;
    }

    if (!playerC.collisionLeft) {
      checkPlayerLeftCollision(playerC, *blockC, velocity, collisionTile);
    }
    if (!playerC.collisionRight) {
      checkPlayerRightCollision(playerC, *blockC, velocity, collisionTile);
    }
    if (!playerC.collisionDown) {
      checkPlayerDownCollision(playerC, *blockC, velocity, collisionTile);
    }
    if (!playerC.collisionUp) {
      checkPlayerUpCollision(playerC, *blockC, velocity, collisionTile);
    }
  }
}

void CollisionDetectionSystem::checkPlayerLeftCollision(CollisionComponent &player,
                                                        CollisionComponent const &block,
                                                        Velocity velocity,
                                                        TILE const &collisionTile) const {
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

  if (Physics::playerCollisionDown(player.collisionRect, block.collisionRect, velocity)) {
    player.yposDif = (player.collisionRect.y + player.collisionRect.h) - block.collisionRect.y;
    player.collisionTileDown = collisionTile;
    player.collidedDownTilePos = {block.collisionRect.x, block.collisionRect.y};
    player.collisionDown = true;
  }
}

void CollisionDetectionSystem::resetPlayerCollisions(
    CollisionComponent &playerCollisionComponent) const {
  playerCollisionComponent.collisionLeft = false;
  playerCollisionComponent.collisionRight = false;
  playerCollisionComponent.collisionUp = false;
  playerCollisionComponent.collisionDown = false;

  playerCollisionComponent.collisionTileDown = TILE::NONE;
  playerCollisionComponent.collisionTileLeft = TILE::NONE;
  playerCollisionComponent.collisionTileRight = TILE::NONE;
  playerCollisionComponent.collisionTileUp = TILE::NONE;
}

void CollisionDetectionSystem::updatePlayerCollisionRect(
    CollisionComponent &playerCollisionComponent,
    TransformComponent const &playerTransformComponent) const {
  playerCollisionComponent.collisionRect.x = playerTransformComponent.position.x;
  playerCollisionComponent.collisionRect.y = playerTransformComponent.position.y;

  playerCollisionComponent.center = {
      (playerCollisionComponent.collisionRect.x + playerCollisionComponent.collisionRect.w +
       playerCollisionComponent.collisionRect.x) /
          2,
      (playerCollisionComponent.collisionRect.y + playerCollisionComponent.collisionRect.h +
       playerCollisionComponent.collisionRect.y) /
          2};
}
