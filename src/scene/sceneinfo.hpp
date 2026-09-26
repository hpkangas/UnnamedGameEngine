#ifndef SCENEINFO_HPP
#define SCENEINFO_HPP

#include "tilegrid.hpp"
#include <vector>

struct SceneInfo {
  entt::registry registry;

  // O(1) spatial lookup of static-collision tiles by tile coordinate.
  TileGrid staticTileGrid;

  // Render-order lists (back-to-front layering by RenderSystem)
  std::vector<entt::entity> tileRenderIds;
  std::vector<entt::entity> objectRenderIds;
  std::vector<entt::entity> UIRenderIds;
  std::vector<entt::entity> staticCollisionIds;

  // The player entity (formerly hardcoded as id 1)
  entt::entity playerEntity = entt::null;

  int levelLength = 0;
  int levelHeight = 0;

};

#endif
