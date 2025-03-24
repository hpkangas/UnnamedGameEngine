#ifndef TILEDETECTIONSYSTEM_HPP
#define TILEDETECTIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/transformcomponent.hpp"

#include <memory>
#include <unordered_map>

struct TileDetectionSystem {

  void update(std::unordered_map<int, std::unique_ptr<CollisionComponent>> &player,
              TransformComponent &tile, TileComponent tileInfo,
              std::unordered_map<int, std::unique_ptr<ControlComponent>> &controlComponent);
};
#endif
