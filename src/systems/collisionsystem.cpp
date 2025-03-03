#include "collisionsystem.hpp"
#include "../physics.hpp"

void CollisionSystem::update(CollisionComponent &playerC, TransformComponent &playerT,
                             SceneInfo &currentLevel) {

  playerC.collisionRect.x = playerT.position.x + 28;
  playerC.collisionRect.y = playerT.position.y;

  playerC.setCollisionDown(false);
  playerC.setCollisionUp(false);
  playerC.setCollisionRight(false);
  playerC.setCollisionLeft(false);

  int yposDif = 0;
  int xposDif = 0;
  for (int i = 3; i < currentLevel.ccs.size(); i++) {

    // player comes from right
    if (Physics::playerCollisionLeft(playerC, *currentLevel.ccs.at(i), playerT.velocity)) {
      auto &block = *currentLevel.ccs.at(i);
      xposDif = playerC.collisionRect.x - (block.collisionRect.x + block.collisionRect.w);
      playerC.setXposDifferenceWithCollisionObject(xposDif);
      playerC.setCollisionLeft(true);
      continue;
    }

    // player comes from left
    if (Physics::playerCollisionRight(playerC, *currentLevel.ccs.at(i), playerT.velocity)) {
      auto &blockc = *currentLevel.ccs.at(i);
      xposDif = (playerC.collisionRect.x + playerC.collisionRect.w) - blockc.collisionRect.x;
      playerC.setXposDifferenceWithCollisionObject(xposDif);
      playerC.setCollisionRight(true);
      continue;
    }

    // player comes from above
    if (Physics::playerCollisionDown(playerC, *currentLevel.ccs.at(i), playerT.velocity)) {
      auto &blockC = *currentLevel.ccs.at(i);
      yposDif = (playerC.collisionRect.y + playerC.collisionRect.h) - blockC.collisionRect.y;
      playerC.setYposDifferenceWithCollisionObject(yposDif);
      playerC.setCollisionDown(true);

      continue;
    }

    // player comes from below
    // if (Physics::playerCollisionUp(selectedPlayer, *block) == true) {
    //   // playerCollisionUp = true;
    //   continue;
    // }
  }

  //   if (playerCollisionUp) {
  //     // TODO Fix when relevant
  //     player->transformComponent.setYpos(player->transformComponent.getYpos() -
  //                                        player->transformComponent.getYvel());
  //   }
  handleDownCollision(playerT, playerC);

  handleHorizontalCollision(playerT, playerC);
}

void CollisionSystem::handleDownCollision(TransformComponent &tc, CollisionComponent &cc) {

  if (cc.getCollisionDown()) {
    if (cc.getYposDifferenceWithCollisionObject() != 0) {
      tc.setYpos(tc.getYpos() - cc.getYposDifferenceWithCollisionObject());
    }
    tc.setYvel(0);
  }
}

void CollisionSystem::handleHorizontalCollision(TransformComponent &tc, CollisionComponent &cc) {
  const int xposDif = cc.getXposDifferenceWithCollisionObject();

  if (cc.getCollisionLeft() || cc.getCollisionRight()) {
    if (xposDif != 0) {
      tc.setXpos(tc.getXpos() - xposDif);
    }
    tc.setXvel(0);
  }
}
