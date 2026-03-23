#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP
#include "../camera.hpp"
#include "../components/transformcomponent.hpp"
#include "scenecreator.hpp"
#include "../systems/animationsystem.hpp"
#include "../systems/collisiondetectionsystem.hpp"
#include "../systems/collisionsystem.hpp"
#include "../systems/controlsystem.hpp"
#include "../systems/gravitysystem.hpp"
#include "../systems/rendersystem.hpp"
#include "../systems/transformsystem.hpp"

struct Scene {
  bool init();
  void handleEvents();
  void update();
  void render();

  SceneInfo currentLevel;
  [[no_unique_address]]Camera camera;

  int selectedPlayerIndex = 0;
  int selectedPlayer = 0;

   [[no_unique_address]]RenderSystem renderSystem;
   [[no_unique_address]]ControlSystem controlSystem;
   [[no_unique_address]]CollisionDetectionSystem collisionDetectionSystem;
   [[no_unique_address]]CollisionSystem collisionSystem;
   [[no_unique_address]]TransformSystem transformSystem;
   [[no_unique_address]]AnimationSystem animationSystem;
   [[no_unique_address]]GravitySystem gravitySystem;

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
