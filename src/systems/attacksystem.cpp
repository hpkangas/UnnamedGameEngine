#include "attacksystem.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"

void AttackSystem::update() {
  for (int id : currentLevel->attackIds) {

    AttackComponent &attackComponent = *currentLevel->attackComponents.at(id);
    TransformComponent const &transformComponent = *currentLevel->transformComponents.at(id);
    updateAttackComponents(attackComponent, transformComponent);

    if (attackComponent.attacking && attackComponent.currentAttackFrames == 24) {
      checkAttackCollisions(attackComponent, id, transformComponent.facingRight);
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
  if (attackerId == 1) {

    for (int id : currentLevel->healthIds) {
      if (id != attackerId) {

        HealthComponent &healthComponent = *currentLevel->healthComponents.at(id);
        CollisionComponent const &collisionComponent = *currentLevel->collisionComponents.at(id);

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
  } else {
    HealthComponent &healthComponent = *currentLevel->healthComponents.at(1);
    CollisionComponent const &collisionComponent = *currentLevel->collisionComponents.at(1);

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
