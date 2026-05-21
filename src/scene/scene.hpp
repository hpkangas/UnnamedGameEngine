#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP
#include "../camera.hpp"
#include "../components/transformcomponent.hpp"
#include "../systems/aisystem.hpp"
#include "../systems/animationsystem.hpp"
#include "../systems/attacksystem.hpp"
#include "../systems/collisiondetectionsystem.hpp"
#include "../systems/collisionsystem.hpp"
#include "../systems/controlsystem.hpp"
#include "../systems/gravitysystem.hpp"
#include "../systems/healthsystem.hpp"
#include "../systems/rendersystem.hpp"
#include "../systems/transformsystem.hpp"

struct Scene {
  inline static bool isRunning = false;

  bool init();
  void handleEvents();
  void update();
  void render() const;

  SceneInfo currentLevel;
  Camera camera;

  entt::entity selectedPlayer = entt::null;

  RenderSystem renderSystem;
  ControlSystem controlSystem;
  CollisionDetectionSystem collisionDetectionSystem;
  CollisionSystem collisionSystem;
  TransformSystem transformSystem;
  AnimationSystem animationSystem;
  GravitySystem gravitySystem;
  AISystem AISystem;
  AttackSystem attackSystem;
  HealthSystem healthSystem;

  void updateCamera(const TransformComponent &transformComponent) const;
  void centerCamera(const TransformComponent &transformComponent) const;

  // template <typename T> T *GetComponent(int entity) {
  //   for (auto &c : currentLevel.components) {
  //     if (c.first == entity) {
  //       if (T *tc = dynamic_cast<T *>(c.second.get())) {
  //         return tc;
  //       }
  //     }
  //   }
  //   return nullptr;
  // }
};

#endif /* GAME_HPP */
