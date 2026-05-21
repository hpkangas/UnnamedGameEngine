#include "aisystem.hpp"
#include "../components/aicomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../utilities/physics.hpp"

void AISystem::update(SceneInfo &currentLevel) const {
  auto &reg = currentLevel.registry;
  entt::entity playerEntity = currentLevel.playerEntity;

  auto view = reg.view<AIComponent, HealthComponent, AttackComponent, CollisionComponent,
                       TransformComponent>();
  for (auto entity : view) {
    AIComponent &aiComponent = view.get<AIComponent>(entity);
    HealthComponent const &healthComponent = view.get<HealthComponent>(entity);
    AttackComponent &attackComponent = view.get<AttackComponent>(entity);
    CollisionComponent const &collisionComponent = view.get<CollisionComponent>(entity);
    TransformComponent &transformComponent = view.get<TransformComponent>(entity);
    float &velx = transformComponent.velocity.x;
    bool facingRight = transformComponent.facingRight;
    CollisionComponent const &player = reg.get<CollisionComponent>(playerEntity);
    HEALTH_STATE playerHealthState = reg.get<HealthComponent>(playerEntity).healthState;

    determineAIState(aiComponent, attackComponent, healthComponent);

    switch (aiComponent.state) {

    case AI_STATE::DEAD:
      velx = 0;

      break;

    case AI_STATE::WALKING:
      SDL_Rect const *attackRect;

      if (facingRight) {
        velx = 4;
        attackRect = &attackComponent.rightCollisionRect;
      } else {
        velx = -4;
        attackRect = &attackComponent.leftCollisionRect;
      }

      if (collisionComponent.collisionTileRight == TILE::SOLID ||
          collisionComponent.collisionTileDown == TILE::NONE) {
        velx = -4;
      } else if (collisionComponent.collisionTileLeft == TILE::SOLID ||
                 collisionComponent.collisionTileDown == TILE::NONE) {
        velx = 4;
      }

      if (playerHealthState == HEALTH_STATE::ALIVE &&
          Physics::attackCollision(*attackRect, player.collisionRect)) {
        aiComponent.state = AI_STATE::ATTACKING;
        attackComponent.attacking = true;
      }
      break;

    case AI_STATE::ATTACKING:
      if (attackComponent.currentAttackFrames == attackComponent.attackFrames - 1) {
        aiComponent.state = AI_STATE::WALKING;
        attackComponent.currentAttackFrames = 0;
        attackComponent.attacking = false;

      } else {
        velx = 0;
        attackComponent.attacking = true;
        attackComponent.currentAttackFrames++;
      }

      break;

    default:
      break;
    }
  }
}

void AISystem::determineAIState(AIComponent &aiComponent, AttackComponent const &attackComponent,
                                HealthComponent const &healthComponent) const {
  if (healthComponent.healthState == HEALTH_STATE::DEAD) {
    aiComponent.state = AI_STATE::DEAD;
    return;
  }

  if (attackComponent.attacking) {
    aiComponent.state = AI_STATE::ATTACKING;
    return;
  } else {
    aiComponent.state = AI_STATE::WALKING;
    return;
  }
}
