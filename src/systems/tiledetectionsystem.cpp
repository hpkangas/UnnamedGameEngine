#include "tiledetectionsystem.hpp"
#include <iostream>

void TileDetectionSystem::update(
    std::unordered_map<int, std::unique_ptr<TileComponent>> &tiles,
    std::unordered_map<int, std::unique_ptr<CollisionComponent>> &players,
    std::unordered_map<int, std::unique_ptr<TransformComponent>> &tileTransforms,
    std::unordered_map<int, std::unique_ptr<ControlComponent>> &controlComponent) {

  // Probably very expensive cpuwise
  for (int i = 0; i < 3; i++) {
    for (auto &tile : tiles) {

      if (players.at(i)->center.x >= tileTransforms.at(tile.first)->getXpos() &&
          players.at(i)->center.x <= tileTransforms.at(tile.first)->getXpos() + 128 &&
          players.at(i)->center.y >= tileTransforms.at(tile.first)->getYpos() &&
          players.at(i)->center.y <= tileTransforms.at(tile.first)->getYpos() + 448) {
        if (tile.second->tile == TILE::LADDER) {
          controlComponent.at(i)->canClimb = true;
          controlComponent.at(i)->collidedTilePos = {tileTransforms.at(tile.first)->getXpos(),
                                                     tileTransforms.at(tile.first)->getYpos()};
          break;
        }
      }
      // climbable gets overwritten if multiple ladders
      controlComponent.at(i)->canClimb = false;
    }
  }
}