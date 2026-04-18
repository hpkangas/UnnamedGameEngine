#include "attacksystem.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"
#include <iostream>

void AttackSystem::update() {
  for (int id : currentLevel->attackIds) {

    AttackComponent &attackComponent = *currentLevel->attackComponents.at(id);
    TransformComponent const &transformComponent = *currentLevel->transformComponents.at(id);
    updateAttackComponents(attackComponent, transformComponent);

    if (attackComponent.attacking && attackComponent.currentAttackFrames == 24) {
      checkAttackCollisions(attackComponent, id, transformComponent.facingRight);
      std::cout << "---stop attacking---\n";
    }
  }
}

void AttackSystem::updateAttackComponents(AttackComponent &attackComponent,
                                          TransformComponent const &transformComponent) const {
  attackComponent.rightCollisionRect.x = transformComponent.position.x + Constants::PLAYER_WIDTH;
  attackComponent.rightCollisionRect.y = transformComponent.position.y;

  attackComponent.leftCollisionRect.x = transformComponent.position.x - Constants::PLAYER_WIDTH / 2;
  attackComponent.leftCollisionRect.y =
      transformComponent.position.y - Constants::PLAYER_HEIGHT / 2;
}

void AttackSystem::checkAttackCollisions(AttackComponent const &attackComponent, int attackerId,
                                         bool facingRight) {
  for (int id : currentLevel->healthIds) {
    if (id != attackerId) {

      HealthComponent &healthComponent = *currentLevel->healthComponents.at(id);
      CollisionComponent const &collisionComponent = *currentLevel->collisionComponents.at(id);
      std::cout << "attack component: " << attackComponent.rightCollisionRect.x << ", "
                << attackComponent.rightCollisionRect.y << std::endl;

      std::cout << "enemy: " << collisionComponent.collisionRect.x << ", "
                << collisionComponent.collisionRect.y << std::endl;

      if (facingRight) {
        if (healthComponent.healthState == HEALTH_STATE::ALIVE &&
            Physics::attackCollision(collisionComponent.collisionRect,
                                     attackComponent.rightCollisionRect)) {
          healthComponent.damageTaken += attackComponent.damage;
        }
      } else {
        if (healthComponent.healthState == HEALTH_STATE::ALIVE &&
            Physics::attackCollision(collisionComponent.collisionRect,
                                     attackComponent.leftCollisionRect)) {
          healthComponent.damageTaken += attackComponent.damage;
        }
      }
    }
  }
}
