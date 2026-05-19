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
#include <unordered_map>

struct SceneInfo {
  int id = 2;
  std::unordered_map<int, std::unique_ptr<AIComponent>> aiComponents;
  std::unordered_map<int, std::unique_ptr<AnimationComponent>> animationComponents;
  std::unordered_map<int, std::unique_ptr<AttackComponent>> attackComponents;
  std::unordered_map<int, std::unique_ptr<CollisionComponent>> collisionComponents;
  std::unordered_map<int, std::unique_ptr<ControlComponent>> controlComponents;
  std::unordered_map<int, std::unique_ptr<HealthComponent>> healthComponents;
  std::unordered_map<int, std::unique_ptr<GravityComponent>> gravityComponents;
  std::unordered_map<int, std::unique_ptr<RenderComponent>> renderComponents;
  std::unordered_map<int, std::unique_ptr<TileComponent>> tileComponents;
  std::unordered_map<int, std::unique_ptr<TransformComponent>> transformComponents;

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
