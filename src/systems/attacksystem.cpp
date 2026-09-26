#include "attacksystem.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../utilities/physics.hpp"

void AttackSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<AttackComponent>();

  for (auto entity : view) {
    AttackComponent &attackComponent = view.get<AttackComponent>(entity);
    PositionComponent const &transformComponent = reg.get<PositionComponent>(entity);

    updateAttackComponents(attackComponent, transformComponent);

    if (attackComponent.attacking && attackComponent.currentAttackFrames == attackComponent.attackHitDetectionFrame) {
      handleAttacks(attackComponent, entity);
    }
  }
}

void AttackSystem::updateAttackComponents(AttackComponent &attackComponent,
                                          PositionComponent const &transformComponent) const {
  attackComponent.collisionRect.x = transformComponent.position.x;
  attackComponent.collisionRect.y = transformComponent.position.y;
}

void AttackSystem::handleAttacks(AttackComponent const &attackComponent,
                                 entt::entity attackerEntity) {
  auto &reg = currentLevel->registry;
  auto view = reg.view<HealthComponent, CollisionComponent>();
  entt::entity playerEntity = currentLevel->playerEntity;

  if (attackerEntity == playerEntity) {
    for (auto entity : view) {
      if (entity != attackerEntity) {
        HealthComponent &healthComponent = view.get<HealthComponent>(entity);
        CollisionComponent const &collisionComponent = view.get<CollisionComponent>(entity);

        attack(attackComponent, healthComponent, collisionComponent);
      }
    }
  }

  else {
    HealthComponent &healthComponent = view.get<HealthComponent>(playerEntity);
    CollisionComponent const &playerCollisionComponent = view.get<CollisionComponent>(playerEntity);
    attack(attackComponent, healthComponent, playerCollisionComponent);
  }
}

void AttackSystem::attack(AttackComponent const &attackComponent,
                          HealthComponent &healthComponent,
                          CollisionComponent const &collisionComponent) const {

  if (healthComponent.healthState == HEALTH_STATE::ALIVE) {


    if (Physics::attackCollision(collisionComponent.collisionRect,  attackComponent.collisionRect)) {
      healthComponent.damageTaken += attackComponent.damage;
    }
  }
}
