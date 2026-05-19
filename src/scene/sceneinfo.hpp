#ifndef SCENEINFO_HPP
#define SCENEINFO_HPP

#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "componentmap.hpp"
#include "tilegrid.hpp"
#include <vector>

struct SceneInfo {
  int id = 2;

  ComponentMap<AIComponent> aiComponents;
  ComponentMap<AnimationComponent> animationComponents;
  ComponentMap<AttackComponent> attackComponents;
  ComponentMap<CollisionComponent> collisionComponents;
  ComponentMap<ControlComponent> controlComponents;
  ComponentMap<HealthComponent> healthComponents;
  ComponentMap<GravityComponent> gravityComponents;
  ComponentMap<RenderComponent> renderComponents;
  ComponentMap<TileComponent> tileComponents;
  ComponentMap<TransformComponent> transformComponents;

  // O(1) spatial lookup of static-collision tiles by tile coordinate.
  // Populated by SceneCreator after the level CSV has been parsed.
  TileGrid staticTileGrid;

  std::vector<int> animationIds;
  std::vector<int> attackIds;
  std::vector<int> aiIds;
  std::vector<int> collisionIds;
  std::vector<int> controlIds;
  std::vector<int> gravityIds;
  std::vector<int> healthIds;
  std::vector<int> objectRenderIds;
  std::vector<int> staticCollisionIds;
  std::vector<int> tileIds;
  std::vector<int> tileRenderIds;
  std::vector<int> transformIds;
  std::vector<int> UIRenderIds;

  int ids = 0;

  int levelLength = 0;
  int levelHeight = 0;
  int getId() {
    id++;
    return id;
  }

  void resetId() { id = 2; }
};

#endif
