#include "aisystem.hpp"

void AISystem::update(SceneInfo &currentLevel) const {
  for (int id : currentLevel.AIIds) {
    float &velx = currentLevel.transformsComponents.at(id)->velocity.x;
    CollisionComponent const &collisionComponent = *currentLevel.collisionComponents.at(id);

    if (collisionComponent.collisionTileRight == TILE::SOLID ||
        collisionComponent.collisionTileDown == TILE::NONE) {
      velx = -4;
    }

    else if (collisionComponent.collisionTileLeft == TILE::SOLID ||
             collisionComponent.collisionTileDown == TILE::NONE) {
      velx = 4;
    }
  }
}
