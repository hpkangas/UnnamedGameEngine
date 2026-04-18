#include "aisystem.hpp"

void AISystem::update(SceneInfo &currentLevel) const {
  for (int id : currentLevel.aiIds) {
    AIComponent &aiComponent = *currentLevel.aiComponents.at(id);
    HealthComponent const &healthComponent = *currentLevel.healthComponents.at(id);

    if (healthComponent.healthState == HEALTH_STATE::DEAD) {
      aiComponent.on = false;
    }

    if (aiComponent.on) {

      float &velx = currentLevel.transformComponents.at(id)->velocity.x;
      CollisionComponent const &collisionComponent = *currentLevel.collisionComponents.at(id);

      if (collisionComponent.collisionTileRight == TILE::SOLID ||
          collisionComponent.collisionTileDown == TILE::NONE) {
        velx = -4;
      }

      else if (collisionComponent.collisionTileLeft == TILE::SOLID ||
               collisionComponent.collisionTileDown == TILE::NONE) {
        velx = 4;
      }
    } else {
      currentLevel.transformComponents.at(id)->velocity.x = 0;
    }
  }
}
