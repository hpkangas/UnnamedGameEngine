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
#include "componentpool.hpp"
#include "tilegrid.hpp"
#include <vector>

struct SceneInfo {
  int id = 2;

  // Section 4 (sparse-set) storage. Iteration is intrinsic to each pool
  // via begin()/end() over the dense T array, so the parallel id lists
  // that used to mirror each pool ("entities with X") are gone.
  ComponentPool<AIComponent>        aiComponents;
  ComponentPool<AnimationComponent> animationComponents;
  ComponentPool<AttackComponent>    attackComponents;
  ComponentPool<CollisionComponent> collisionComponents;
  ComponentPool<ControlComponent>   controlComponents;
  ComponentPool<HealthComponent>    healthComponents;
  ComponentPool<GravityComponent>   gravityComponents;
  ComponentPool<RenderComponent>    renderComponents;
  ComponentPool<TileComponent>      tileComponents;
  ComponentPool<TransformComponent> transformComponents;

  // O(1) spatial lookup of static-collision tiles by tile coordinate.
  // Populated by SceneCreator after the level CSV has been parsed.
  TileGrid staticTileGrid;

  // The id lists that survive encode meaning the pools alone cannot:
  //   * render order (back-to-front layering by RenderSystem)
  //   * the one-shot list SceneCreator uses to bake staticTileGrid
  std::vector<int> tileRenderIds;
  std::vector<int> objectRenderIds;
  std::vector<int> UIRenderIds;
  std::vector<int> staticCollisionIds;

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
