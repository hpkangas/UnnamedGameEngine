#include "aisystem.hpp"
#include "../utilities/physics.hpp"
#include "SDL_rect.h"

void AISystem::update(SceneInfo &currentLevel) const {
  auto &pool = currentLevel.aiComponents;
  for (std::size_t i = 0; i < pool.size(); ++i) {
    const int id = pool.entityAt(i);

    AIComponent &aiComponent = pool[i];
    HealthComponent const &healthComponent = currentLevel.healthComponents.at(id);
    AttackComponent &attackComponent = currentLevel.attackComponents.at(id);
    CollisionComponent const &collisionComponent = currentLevel.collisionComponents.at(id);
    // Section 2.2: cache the transform reference once instead of looking
    // it up twice for velocity and facingRight.
    TransformComponent &transformComponent = currentLevel.transformComponents.at(id);
    float &velx = transformComponent.velocity.x;
    bool facingRight = transformComponent.facingRight;
    CollisionComponent const &player = currentLevel.collisionComponents.at(1);
    HEALTH_STATE playerHealthState = currentLevel.healthComponents.at(1).healthState;

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
