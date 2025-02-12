#pragma once
#ifndef GAME_HPP
#define GAME_HPP
#include "camera.hpp"
#include "entities/player.hpp"
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
  void render(SDL_Renderer &renderer);

private:
  Player *selectedPlayer = nullptr;
  Level level;
  LevelInfo currentLevel;
  Camera camera;

  RenderSystem renderSystem;
  ControlSystem controlSystem;
  CollisionSystem collisionSystem;
  TransformSystem transformSystem;
  AnimationSystem animationSystem;
  GravitySystem gravitySystem;

  void updateCamera(const Player &player);
};

#endif /* GAME_HPP */