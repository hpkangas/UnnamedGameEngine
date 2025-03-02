#pragma once
#ifndef GAME_HPP
#define GAME_HPP
#include "camera.hpp"
#include "components/transformcomponent.hpp"
#include "level.hpp"
#include "systems/animationsystem.hpp"
#include "systems/collisionsystem.hpp"
#include "systems/controlsystem.hpp"
#include "systems/gravitysystem.hpp"
#include "systems/rendersystem.hpp"
#include "systems/transformsystem.hpp"

class Game {
public:
  void init();
  void handleEvents();
  void update();
  void render();

private:
  Level level;
  LevelInfo currentLevel;
  Camera camera;

  int selectedPlayerIndex = 0;
  int selectedPlayer = 0;

  RenderSystem renderSystem;
  ControlSystem controlSystem;
  CollisionSystem collisionSystem;
  TransformSystem transformSystem;
  AnimationSystem animationSystem;
  GravitySystem gravitySystem;

  void updateCamera(TransformComponent &transformComponent);
  void centerCamera(TransformComponent &transformComponent);

  template <typename T> T *GetComponent(int entity) {
    for (auto &c : currentLevel.components) {
      if (c.first == entity) {
        if (T *tc = dynamic_cast<T *>(c.second.get())) {
          return tc;
        }
      }
    }
    return nullptr;
  }
};

#endif /* GAME_HPP */
