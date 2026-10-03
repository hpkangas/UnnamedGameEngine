#ifndef SCENEINFO_HPP
#define SCENEINFO_HPP

#include "tilegrid.hpp"

struct SceneInfo {
  entt::registry registry;

  TileGrid staticTileGrid;

  entt::entity playerEntity = entt::null;

  int levelLength = 0;
  int levelHeight = 0;

};

#endif
