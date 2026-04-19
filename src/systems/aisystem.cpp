#include "aisystem.hpp"
#include "../utilities/physics.hpp"

void AISystem::update(SceneInfo &currentLevel) const {
  for (int id : currentLevel.aiIds) {

    AIComponent &aiComponent = *currentLevel.aiComponents.at(id);
    HealthComponent const &healthComponent = *currentLevel.healthComponents.at(id);
    AttackComponent &attackComponent = *currentLevel.attackComponents.at(id);
    CollisionComponent const &collisionComponent = *currentLevel.collisionComponents.at(id);
    float &velx = currentLevel.transformComponents.at(id)->velocity.x;
    bool facingRight = currentLevel.transformComponents.at(id)->facingRight;
    CollisionComponent const &player = *currentLevel.collisionComponents.at(1);
    HEALTH_STATE playerHealthState = currentLevel.healthComponents.at(1)->healthState;

    determineAIState(aiComponent, attackComponent, healthComponent);

    switch (aiComponent.state) {

    case AI_STATE::DEAD:
      velx = 0;

      break;

    case AI_STATE::WALKING:
      if (facingRight) {
        velx = 4;
      } else {
        velx = -4;
      }

      if (collisionComponent.collisionTileRight == TILE::SOLID ||
          collisionComponent.collisionTileDown == TILE::NONE) {
        velx = -4;
      }

      else if (collisionComponent.collisionTileLeft == TILE::SOLID ||
               collisionComponent.collisionTileDown == TILE::NONE) {
        velx = 4;
      }

      if (playerHealthState == HEALTH_STATE::ALIVE &&
          Physics::attackCollision(collisionComponent.collisionRect, player.collisionRect)) {
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
