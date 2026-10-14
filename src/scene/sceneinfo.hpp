#ifndef SCENEINFO_HPP
#define SCENEINFO_HPP

#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "tilegrid.hpp"

struct SceneInfo {
  entt::registry registry;

  std::map<entt::entity, Uint64> entitiesToBeDestroyed;

  TileGrid staticTileGrid;

  entt::entity playerEntity = entt::null;

  int levelLength = 0;
  int levelHeight = 0;

  void destroyEntity(entt::entity entity) { entitiesToBeDestroyed.emplace(entity, SDL_GetTicks64()); }

};

#endif
