#include "collisionsystem.hpp"
#include "../physics.hpp"

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

  playerC.setCollisionDown(false);
  playerC.setCollisionUp(false);
  playerC.setCollisionRight(false);
  playerC.setCollisionLeft(false);

  int yposDif = 0;
  int xposDif = 0;

  // for (int i = 3; i < currentLevel.ccs.size(); i++) {
  for (int i = 3; i < currentLevel.tiles.size(); i++) {
    //if check or list that is defined in scene creation
    //what if list changes during run?
    if (currentLevel.tiles.at(i)->tile == TILE::BLOCKING) {

      auto &blockC = currentLevel.ccs.at(i);

      // auto &blockC = *currentLevel.ccs.at(i);

      // player comes from right
      if (Physics::playerCollisionLeft(playerC, *blockC, playerT.velocity)) {
        xposDif = playerC.collisionRect.x - (blockC->collisionRect.x + blockC->collisionRect.w);
        playerC.setCollisionLeft(true);
        continue;
      }

      // player comes from left
      if (Physics::playerCollisionRight(playerC, *blockC, playerT.velocity)) {
        xposDif = (playerC.collisionRect.x + playerC.collisionRect.w) - blockC->collisionRect.x;
        playerC.setCollisionRight(true);
        continue;
      }

      // player comes from above
      if (Physics::playerCollisionDown(playerC, *blockC, playerT.velocity)) {
        yposDif = (playerC.collisionRect.y + playerC.collisionRect.h) - blockC->collisionRect.y;
        playerC.setCollisionDown(true);

        continue;
      }

      // player comes from below
      if (Physics::playerCollisionUp(playerC, *blockC, playerT.velocity)) {
        yposDif = (playerC.collisionRect.y) - (blockC->collisionRect.y + blockC->collisionRect.h);

        playerC.setCollisionUp(true);
        continue;
      }

      //   if (playerCollisionUp) {
      //     // TODO Fix when relevant
      //     player->transformComponent.setYpos(player->transformComponent.getYpos() -
      //                                        player->transformComponent.getYvel());
      //   }
    }
  }
  handleVerticalBlockCollision(playerT, playerC, yposDif);

  handleHorizontalBlockCollision(playerT, playerC, xposDif);
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
