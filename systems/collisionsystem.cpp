#include "collisionsystem.hpp"
#include "../physics.hpp"

void CollisionSystem::update(Player &selectedPlayer, LevelInfo &currentLevel) {
  for (auto &&player : currentLevel.playercharacters) {

    selectedPlayer.collisionComponent.collisionRect.x = selectedPlayer.transformComponent.position.x + 28;
    selectedPlayer.collisionComponent.collisionRect.y = selectedPlayer.transformComponent.position.y;

    player->collisionComponent.setCollisionDown(false);
    player->collisionComponent.setCollisionUp(false);
    player->collisionComponent.setCollisionRight(false);
    player->collisionComponent.setCollisionLeft(false);

    int yposDif = 0;
    int xposDif = 0;

    for (const auto &block : currentLevel.blocks) {

      // player comes from right
      if (Physics::playerCollisionLeft(*player, *block) == true) {
        xposDif =
            player->collisionComponent.collisionRect.x -
            (block->collisionComponent.collisionRect.x + block->collisionComponent.collisionRect.w);
        player->collisionComponent.setXposDifferenceWithCollisionObject(xposDif);
        player->collisionComponent.setCollisionLeft(true);
        continue;
      }

      // player comes from left
      if (Physics::playerCollisionRight(*player, *block) == true) {
        xposDif = (player->collisionComponent.collisionRect.x +
                   player->collisionComponent.collisionRect.w) -
                  block->collisionComponent.collisionRect.x;
        player->collisionComponent.setXposDifferenceWithCollisionObject(xposDif);
        player->collisionComponent.setCollisionRight(true);
        continue;
      }

      // player comes from above
      if (Physics::playerCollisionDown(*player, *block) == true) {
        yposDif = (player->collisionComponent.collisionRect.y +
                   player->collisionComponent.collisionRect.h) -
                  block->collisionComponent.collisionRect.y;
        player->collisionComponent.setYposDifferenceWithCollisionObject(yposDif);
        player->collisionComponent.setCollisionDown(true);

        continue;
      }

      // player comes from below
      if (Physics::playerCollisionUp(*player, *block) == true) {
        // playerCollisionUp = true;
        continue;
      }
    }

    //   if (playerCollisionUp) {
    //     // TODO Fix when relevant
    //     player->transformComponent.setYpos(player->transformComponent.getYpos() -
    //                                        player->transformComponent.getYvel());
    //   }
    handleDownCollision(*player);

    handleHorizontalCollision(*player);
  }
}

void CollisionSystem::handleDownCollision(Player &selectedPlayer) {
  if (selectedPlayer.collisionComponent.getCollisionDown()) {
    if (selectedPlayer.collisionComponent.getYposDifferenceWithCollisionObject() != 0)
      selectedPlayer.transformComponent.setYpos(
          selectedPlayer.transformComponent.getYpos() -
          selectedPlayer.collisionComponent.getYposDifferenceWithCollisionObject());
    selectedPlayer.transformComponent.setYvel(0);
  }
}

void CollisionSystem::handleHorizontalCollision(Player &selectedPlayer) {
  const int xposDif = selectedPlayer.collisionComponent.getXposDifferenceWithCollisionObject();

  if (selectedPlayer.collisionComponent.getCollisionLeft() ||
      selectedPlayer.collisionComponent.getCollisionRight()) {
    if (xposDif != 0) {
      selectedPlayer.transformComponent.setXpos(selectedPlayer.transformComponent.getXpos() -
                                                xposDif);
    }
    selectedPlayer.transformComponent.setXvel(0);
  }
}