#include "attacksystem.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"

void AttackSystem::update() {
  // Driver loop iterates the attack pool directly; entityAt(i) recovers
  // the entity id we need for the player-vs-enemy branching below.
  auto &pool = currentLevel->attackComponents;
  for (std::size_t i = 0; i < pool.size(); ++i) {
    AttackComponent &attackComponent = pool[i];
    const int eid = pool.entityAt(i);
    TransformComponent const &transformComponent = currentLevel->transformComponents.at(eid);
    updateAttackComponents(attackComponent, transformComponent);

    if (attackComponent.attacking && attackComponent.currentAttackFrames == 24) {
      checkAttackCollisions(attackComponent, eid, transformComponent.facingRight);
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
    // Player attacks any enemy with a health component. Iterate the
    // pool directly and skip the attacker by id.
    auto &healths = currentLevel->healthComponents;
    for (std::size_t i = 0; i < healths.size(); ++i) {
      const int eid = healths.entityAt(i);
      if (eid == attackerId) continue;

      HealthComponent &healthComponent = healths[i];
      CollisionComponent const &collisionComponent = currentLevel->collisionComponents.at(eid);

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

  } else {
    HealthComponent &healthComponent = currentLevel->healthComponents.at(1);
    CollisionComponent const &collisionComponent = currentLevel->collisionComponents.at(1);

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
