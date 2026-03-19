#include "collisiondetectionsystem.hpp"
#include "../physics.hpp"
#include <cstdio>

void CollisionDetectionSystem::update(int i, SceneInfo &currentLevel) {
  checkBlockCollision(i, currentLevel);
}

void CollisionDetectionSystem::checkBlockCollision(int i, SceneInfo &currentLevel) {
  CollisionComponent &playerC = *currentLevel.ccs.at(i);
  TransformComponent &playerT = *currentLevel.tcs.at(i);

  updatePlayerCollisionRect(playerC, playerT);
  resetPlayerCollisions(playerC);
  Velocity velocity = playerT.getVelocity();
  // other than player ids
  for (int j : currentLevel.collisionIds) {
    if (j==0) {
    j++;
    }
    auto &blockC = currentLevel.ccs.at(j);
    TILE collisionTile = currentLevel.tiles.at(j)->getTile();

    if (!playerC.getCollisionLeft()) {
      checkPlayerLeftCollision(playerC, *blockC, velocity, collisionTile);
    }
    if (!playerC.getCollisionRight()) {
      checkPlayerRightCollision(playerC, *blockC, velocity, collisionTile);
      
    }
    if (!playerC.getCollisionDown()) {
      checkPlayerDownCollision(playerC, *blockC, velocity, collisionTile);}
      if (!playerC.getCollisionUp()) {
    checkPlayerUpCollision(playerC, *blockC, velocity, collisionTile);}
  }
}

void CollisionDetectionSystem::checkPlayerLeftCollision(CollisionComponent &player,
                                                        CollisionComponent &block,
                                                        Velocity velocity, TILE &collisionTile) {
  if (Physics::playerCollisionLeft(player.collisionRect, block.collisionRect, velocity)) {
    player.xposDif = player.collisionRect.x - (block.collisionRect.x + block.collisionRect.w);
    player.collisionTileLeft = collisionTile;

    player.setCollisionLeft(true);
  }
}

void CollisionDetectionSystem::checkPlayerRightCollision(CollisionComponent &player,
                                                         CollisionComponent &block,
                                                         Velocity velocity, TILE &collisionTile) {
  if (Physics::playerCollisionRight(player.collisionRect, block.collisionRect, velocity)) {
    player.xposDif = (player.collisionRect.x + player.collisionRect.w) - block.collisionRect.x;
    player.collisionTileRight = collisionTile;

    player.setCollisionRight(true);
  }
}

void CollisionDetectionSystem::checkPlayerUpCollision(CollisionComponent &player,
                                                      CollisionComponent &block, Velocity velocity,
                                                      TILE &collisionTile) {
  if (Physics::playerCollisionUp(player.collisionRect, block.collisionRect, velocity)) {
    player.yposDif = (player.collisionRect.y) - (block.collisionRect.y + block.collisionRect.h);
    player.collisionTileUp = collisionTile;
    player.collidedUpTilePos = {block.collisionRect.x, block.collisionRect.y};
    player.setCollisionUp(true);
  }
}

void CollisionDetectionSystem::checkPlayerDownCollision(CollisionComponent &player,
                                                        CollisionComponent &block,
                                                        Velocity velocity, TILE &collisionTile) {

  if (Physics::playerCollisionDown(player.collisionRect, block.collisionRect, velocity)) {
    player.yposDif = (player.collisionRect.y + player.collisionRect.h) - block.collisionRect.y;
    player.collisionTileDown = collisionTile;
    player.collidedDownTilePos = {block.collisionRect.x, block.collisionRect.y};
    player.setCollisionDown(true);
  }
}

void CollisionDetectionSystem::resetPlayerCollisions(CollisionComponent &playerCollisionComponent) {
  playerCollisionComponent.setCollisionLeft(false);
  playerCollisionComponent.setCollisionRight(false);
  playerCollisionComponent.setCollisionUp(false);
  playerCollisionComponent.setCollisionDown(false);

  playerCollisionComponent.collisionTileDown = TILE::NONE;
  playerCollisionComponent.collisionTileLeft = TILE::NONE;
  playerCollisionComponent.collisionTileRight = TILE::NONE;
  playerCollisionComponent.collisionTileUp = TILE::NONE;
}

void CollisionDetectionSystem::updatePlayerCollisionRect(
    CollisionComponent &playerCollisionComponent, TransformComponent &playerTransformComponent) {
  playerCollisionComponent.collisionRect.x = playerTransformComponent.getXpos();
  playerCollisionComponent.collisionRect.y = playerTransformComponent.getYpos();

  playerCollisionComponent.center = {
      (playerCollisionComponent.collisionRect.x + playerCollisionComponent.collisionRect.w +
       playerCollisionComponent.collisionRect.x) /
          2,
      (playerCollisionComponent.collisionRect.y + playerCollisionComponent.collisionRect.h +
       playerCollisionComponent.collisionRect.y) /
          2};
}
