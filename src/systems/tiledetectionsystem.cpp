#include "tiledetectionsystem.hpp"

void TileDetectionSystem::update(
    std::unordered_map<int, std::unique_ptr<CollisionComponent>> &players,
    TransformComponent &tile, TileComponent tileInfo,
    std::unordered_map<int, std::unique_ptr<ControlComponent>> &controlComponent) {

  for (int i = 0; i < 3; i++) {
    if (players.at(i)->center.x > tile.getXpos() &&
        players.at(i)->center.x < tile.getXpos() + 128 &&
        players.at(i)->center.y > tile.getYpos() &&
        players.at(i)->center.y < tile.getYpos() + 448) {
      if (tileInfo.tile == LADDER) {
        controlComponent.at(i)->canClimb = true;
        continue;
      }
    }
    controlComponent.at(i)->canClimb = false;
  }
}
