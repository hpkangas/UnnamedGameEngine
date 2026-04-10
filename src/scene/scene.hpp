#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP
#include "../camera.hpp"
#include "../components/transformcomponent.hpp"
#include "../systems/aisystem.hpp"
#include "../systems/animationsystem.hpp"
#include "../systems/collisiondetectionsystem.hpp"
#include "../systems/collisionsystem.hpp"
#include "../systems/controlsystem.hpp"
#include "../systems/gravitysystem.hpp"
#include "../systems/rendersystem.hpp"
#include "../systems/transformsystem.hpp"
#include "../utilities/constants.hpp"

struct Scene {
  inline static bool isRunning = false;

  bool init();
  void handleEvents();
  void update();
  void render() const;

  SceneInfo currentLevel;
  [[no_unique_address]] Camera camera;

  int selectedPlayerIndex = Constants::PLAYER_INDEX;
  int selectedPlayer = Constants::PLAYER_INDEX;

  [[no_unique_address]] RenderSystem renderSystem;
  [[no_unique_address]] ControlSystem controlSystem;
  CollisionDetectionSystem collisionDetectionSystem;
  [[no_unique_address]] CollisionSystem collisionSystem;
  [[no_unique_address]] TransformSystem transformSystem;
  AnimationSystem animationSystem;
  [[no_unique_address]] GravitySystem gravitySystem;
  [[no_unique_address]] AISystem AISystem;

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
