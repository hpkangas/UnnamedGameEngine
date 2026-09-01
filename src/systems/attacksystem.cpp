#include "attacksystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../utilities/constants.hpp"
#include "../utilities/physics.hpp"

void AttackSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<AttackComponent, TransformComponent>();

  for (auto entity : view) {
    AttackComponent &attackComponent = view.get<AttackComponent>(entity);
    TransformComponent const &transformComponent = view.get<TransformComponent>(entity);

    updateAttackComponents(attackComponent, transformComponent);

    //TODO magic number
    if (attackComponent.attacking && attackComponent.currentAttackFrames == 24) {
      handleAttacks(attackComponent, entity, transformComponent.facingRight);
    }
  }
}

void AttackSystem::updateAttackComponents(AttackComponent &attackComponent,
                                          TransformComponent const &transformComponent) const {
  attackComponent.rightCollisionRect.x = transformComponent.position.x;
  attackComponent.rightCollisionRect.y = transformComponent.position.y;

  attackComponent.leftCollisionRect.x = transformComponent.position.x;
  attackComponent.leftCollisionRect.y =
      transformComponent.position.y;
}

void AttackSystem::handleAttacks(AttackComponent const &attackComponent,
                                 entt::entity attackerEntity, bool facingRight) {
  auto &reg = currentLevel->registry;
  auto view = reg.view<HealthComponent, CollisionComponent>();
  entt::entity playerEntity = currentLevel->playerEntity;

  if (attackerEntity == playerEntity) {
    for (auto entity : view) {
      if (entity != attackerEntity) {
        HealthComponent &healthComponent = view.get<HealthComponent>(entity);
        CollisionComponent const &collisionComponent = view.get<CollisionComponent>(entity);

        attack(attackComponent, facingRight, healthComponent, collisionComponent);
      }
    }
  }

  else {
    HealthComponent &healthComponent = view.get<HealthComponent>(playerEntity);
    CollisionComponent const &playerCollisionComponent = view.get<CollisionComponent>(playerEntity);
    attack(attackComponent, facingRight, healthComponent, playerCollisionComponent);
  }
}

void AttackSystem::attack(AttackComponent const &attackComponent, bool facingRight,
                          HealthComponent &healthComponent,
                          CollisionComponent const &collisionComponent) const {

  if (healthComponent.healthState == HEALTH_STATE::ALIVE) {

    SDL_Rect attackCollision;
    if (facingRight) {
      attackCollision = attackComponent.rightCollisionRect;
    } else {
      attackCollision = attackComponent.leftCollisionRect;
    }

    if (Physics::attackCollision(collisionComponent.collisionRect, attackCollision)) {
      healthComponent.damageTaken += attackComponent.damage;
    }
  }
}
