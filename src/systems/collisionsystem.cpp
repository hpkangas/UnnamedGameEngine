#include "collisionsystem.hpp"
#include "../physics.hpp"

void CollisionSystem::update(Entity &selectedPlayer, LevelInfo &currentLevel) {

  selectedPlayer.GetComponent<CollisionComponent>()->collisionRect.x =
      selectedPlayer.GetComponent<TransformComponent>()->position.x + 28;
  selectedPlayer.GetComponent<CollisionComponent>()->collisionRect.y =
      selectedPlayer.GetComponent<TransformComponent>()->position.y;

  selectedPlayer.GetComponent<CollisionComponent>()->setCollisionDown(false);
  selectedPlayer.GetComponent<CollisionComponent>()->setCollisionUp(false);
  selectedPlayer.GetComponent<CollisionComponent>()->setCollisionRight(false);
  selectedPlayer.GetComponent<CollisionComponent>()->setCollisionLeft(false);

  int yposDif = 0;
  int xposDif = 0;

  for (const auto &block : currentLevel.entities) {

    // TODO How to not collide with other PCs?

    // player comes from right
    if (Physics::playerCollisionLeft(selectedPlayer, *block) == true) {
      auto playerc = selectedPlayer.GetComponent<CollisionComponent>();
      auto blockc = block->GetComponent<CollisionComponent>();
      xposDif = playerc->collisionRect.x - (blockc->collisionRect.x + blockc->collisionRect.w);
      playerc->setXposDifferenceWithCollisionObject(xposDif);
      playerc->setCollisionLeft(true);
      continue;
    }

    // player comes from left
    if (Physics::playerCollisionRight(selectedPlayer, *block) == true) {
      auto playerc = selectedPlayer.GetComponent<CollisionComponent>();
      auto blockc = block->GetComponent<CollisionComponent>();
      xposDif = (playerc->collisionRect.x + playerc->collisionRect.w) - blockc->collisionRect.x;
      playerc->setXposDifferenceWithCollisionObject(xposDif);
      playerc->setCollisionRight(true);
      continue;
    }

    // player comes from above
    if (Physics::playerCollisionDown(selectedPlayer, *block) == true) {
      auto playerc = selectedPlayer.GetComponent<CollisionComponent>();
      auto blockc = block->GetComponent<CollisionComponent>();
      yposDif = (playerc->collisionRect.y + playerc->collisionRect.h) - blockc->collisionRect.y;
      playerc->setYposDifferenceWithCollisionObject(yposDif);
      playerc->setCollisionDown(true);

      continue;
    }

    // player comes from below
    if (Physics::playerCollisionUp(selectedPlayer, *block) == true) {
      // playerCollisionUp = true;
      continue;
    }
  }

  //   if (playerCollisionUp) {
  //     // TODO Fix when relevant
  //     player->transformComponent.setYpos(player->transformComponent.getYpos() -
  //                                        player->transformComponent.getYvel());
  //   }
  handleDownCollision(selectedPlayer);

  handleHorizontalCollision(selectedPlayer);
}

void CollisionSystem::handleDownCollision(Entity &selectedPlayer) {
  auto playerc = selectedPlayer.GetComponent<CollisionComponent>();

  if (playerc->getCollisionDown()) {
    if (playerc->getYposDifferenceWithCollisionObject() != 0)
      selectedPlayer.GetComponent<TransformComponent>()->setYpos(
          selectedPlayer.GetComponent<TransformComponent>()->getYpos() -
          playerc->getYposDifferenceWithCollisionObject());
    selectedPlayer.GetComponent<TransformComponent>()->setYvel(0);
  }
}

void CollisionSystem::handleHorizontalCollision(Entity &selectedPlayer) {
  auto playerc = selectedPlayer.GetComponent<CollisionComponent>();
  const int xposDif = playerc->getXposDifferenceWithCollisionObject();

  if (playerc->getCollisionLeft() || playerc->getCollisionRight()) {
    if (xposDif != 0) {
      selectedPlayer.GetComponent<TransformComponent>()->setXpos(
          selectedPlayer.GetComponent<TransformComponent>()->getXpos() - xposDif);
    }
    selectedPlayer.GetComponent<TransformComponent>()->setXvel(0);
  }
}