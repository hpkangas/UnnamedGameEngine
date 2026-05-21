#include "attacksystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"

void AttackSystem::update() {
  auto &reg = currentLevel->registry;
  entt::entity playerEntity = currentLevel->playerEntity;

  auto view = reg.view<AttackComponent, TransformComponent>();
  for (auto entity : view) {
    AttackComponent &attackComponent = view.get<AttackComponent>(entity);
    TransformComponent const &transformComponent = view.get<TransformComponent>(entity);
    updateAttackComponents(attackComponent, transformComponent);

    if (attackComponent.attacking && attackComponent.currentAttackFrames == 24) {
      checkAttackCollisions(attackComponent, entity, transformComponent.facingRight);
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

void AttackSystem::checkAttackCollisions(AttackComponent const &attackComponent,
                                         entt::entity attackerEntity, bool facingRight) {
  auto &reg = currentLevel->registry;
  entt::entity playerEntity = currentLevel->playerEntity;

  if (attackerEntity == playerEntity) {
    // Player attacks any enemy with a health component.
    auto view = reg.view<HealthComponent, CollisionComponent>();
    for (auto entity : view) {
      if (entity == attackerEntity) {
        continue;
      }

      HealthComponent &healthComponent = view.get<HealthComponent>(entity);
      CollisionComponent const &collisionComponent = view.get<CollisionComponent>(entity);

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
    HealthComponent &healthComponent = reg.get<HealthComponent>(playerEntity);
    CollisionComponent const &collisionComponent = reg.get<CollisionComponent>(playerEntity);

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
