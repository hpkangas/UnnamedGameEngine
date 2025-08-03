#include "collisionsystem.hpp"
#include "../physics.hpp"
#include <cstdio>
#include <iostream>

void CollisionSystem::update(CollisionComponent &playerC, TransformComponent &playerT,
                             SceneInfo &currentLevel) {
  checkBlockCollision(playerC, playerT, currentLevel);
}

void CollisionSystem::checkBlockCollision(CollisionComponent &playerC, TransformComponent &playerT,
                                          SceneInfo &currentLevel) {

  playerC.collisionRect.x = playerT.position.x + 28;
  playerC.collisionRect.y = playerT.position.y;
  playerC.center = {
      (playerC.collisionRect.x + playerC.collisionRect.w + playerC.collisionRect.x) / 2,
      (playerC.collisionRect.y + playerC.collisionRect.h + playerC.collisionRect.y) / 2};

  playerC.setCollisionLeft(false);
  playerC.setCollisionRight(false);
  playerC.setCollisionUp(false);
  playerC.setCollisionDown(false);

  yposDif = 0;
  xposDif = 0;

  // for (int i = 3; i < currentLevel.ccs.size(); i++) {
  for (int i = 3; i < currentLevel.tiles.size(); i++) {
    // if check or list that is defined in scene creation
    // what if list changes during run?
    printf("%d",currentLevel.tiles.at(i)->tile == TILE::LADDER);
    // LADDER does not have collision component
    if (currentLevel.tiles.at(i)->tile == TILE::LADDER) {
      auto &blockC = currentLevel.ccs.at(i);
      std::cout<<"asd";
      checkPlayerDownCollision(playerC, *blockC, playerT.velocity);
    }

    if (currentLevel.tiles.at(i)->tile == TILE::SOLID) {
      auto &blockC = currentLevel.ccs.at(i);

      checkPlayerLeftCollision(playerC, *blockC, playerT.velocity);
      checkPlayerRightCollision(playerC, *blockC, playerT.velocity);
      checkPlayerUpCollision(playerC, *blockC, playerT.velocity);
      checkPlayerDownCollision(playerC, *blockC, playerT.velocity);
    }

    if (currentLevel.tiles.at(i)->tile == TILE::SEMISOLID) {
      auto &blockC = currentLevel.ccs.at(i);

      checkPlayerLeftCollision(playerC, *blockC, playerT.velocity);
      checkPlayerRightCollision(playerC, *blockC, playerT.velocity);
      checkPlayerDownCollision(playerC, *blockC, playerT.velocity);
    }
  }
  handleVerticalBlockCollision(playerT, playerC, yposDif);

  handleHorizontalBlockCollision(playerT, playerC, xposDif);
}

void CollisionSystem::checkPlayerLeftCollision(CollisionComponent &player,
                                               CollisionComponent &block, Velocity velocity) {
  if (Physics::playerCollisionLeft(player, block, velocity)) {
    xposDif = player.collisionRect.x - (block.collisionRect.x + block.collisionRect.w);
    player.setCollisionLeft(true);
  }
}

void CollisionSystem::checkPlayerRightCollision(CollisionComponent &player,
                                                CollisionComponent &block, Velocity velocity) {
  if (Physics::playerCollisionRight(player, block, velocity)) {
    xposDif = (player.collisionRect.x + player.collisionRect.w) - block.collisionRect.x;
    player.setCollisionRight(true);
  }
}

void CollisionSystem::checkPlayerUpCollision(CollisionComponent &player, CollisionComponent &block,
                                             Velocity velocity) {
  if (Physics::playerCollisionUp(player, block, velocity)) {
    yposDif = (player.collisionRect.y) - (block.collisionRect.y + block.collisionRect.h);
    player.setCollisionUp(true);
  }
}

void CollisionSystem::checkPlayerDownCollision(CollisionComponent &player,
                                               CollisionComponent &block, Velocity velocity) {
  if (Physics::playerCollisionDown(player, block, velocity)) {
    yposDif = (player.collisionRect.y + player.collisionRect.h) - block.collisionRect.y;
    player.setCollisionDown(true);
  }
}

void CollisionSystem::handleVerticalBlockCollision(TransformComponent &tc, CollisionComponent &cc,
                                                   int dif) {

  if (cc.getCollisionDown() || cc.getCollisionUp()) {
    if (dif != 0) {
      tc.setYpos(tc.getYpos() - dif);
    }
    tc.setYvel(0);
  }
}

void CollisionSystem::handleHorizontalBlockCollision(TransformComponent &tc, CollisionComponent &cc,
                                                     int dif) {

  if (cc.getCollisionLeft() || cc.getCollisionRight()) {
    if (dif != 0) {
      tc.setXpos(tc.getXpos() - dif);
    }
    tc.setXvel(0);
  }
}
