#include "attacksystem.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"
#include <iostream>

void AttackSystem::update() {
  for (int id : currentLevel->attackIds) {

    AttackComponent &attackComponent = *currentLevel->attackComponents.at(id);
    TransformComponent &transformComponent = *currentLevel->transformComponents.at(id);
    updateAttackComponents(attackComponent, transformComponent);

    if (attackComponent.attacking) {
      checkAttackCollisions(attackComponent, id);
    }
  }
}

void AttackSystem::updateAttackComponents(AttackComponent &attackComponent,
                                          TransformComponent &transformComponent) {
  attackComponent.collisionRect.x = transformComponent.position.x + Constants::PLAYER_WIDTH;
  attackComponent.collisionRect.y = transformComponent.position.y + Constants::PLAYER_HEIGHT;
}

void AttackSystem::checkAttackCollisions(AttackComponent const &attackComponent, int attackerId) {
  for (int id : currentLevel->healthIds) {
    if (id != attackerId) {

      HealthComponent &healthComponent = *currentLevel->healthComponents.at(id);
      CollisionComponent &collisionComponent = *currentLevel->collisionComponents.at(id);
      if (Physics::checkCollision(collisionComponent.collisionRect,
                                  attackComponent.collisionRect)) {
        std::cout << "ATTACK!! " << id << std::endl;
        healthComponent.damageTaken += attackComponent.damage;
      } else {
        std::cout << "no contact to id " << id << std::endl;
      }
    }
  }
}
