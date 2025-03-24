#include "tiledetectionsystem.hpp"
#include <iostream>

void TileDetectionSystem::update(
    std::unordered_map<int, std::unique_ptr<CollisionComponent>> &player,
    TransformComponent &tileCollision, TileComponent tile,
    std::unordered_map<int, std::unique_ptr<ControlComponent>> &controlComponent) {

  for (int i = 0; i < 3; i++) {
    if (player.at(i)->center.x > tileCollision.getXpos() &&
        player.at(i)->center.x < tileCollision.getXpos() + 128 &&
        player.at(i)->center.y > tileCollision.getYpos() &&
        player.at(i)->center.y < tileCollision.getYpos() + 448) {
      controlComponent.at(i)->canClimb = true;
    } else {
      controlComponent.at(i)->canClimb = false;
    }
  }
}
