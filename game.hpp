#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "camera.hpp"
#include "entities/player.hpp"
#include "level.hpp"
#include "systems/controlsystem.hpp"
#include "systems/rendersystem.hpp"

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

  bool playerCollisionUp = false;

  void checkPlayerMovementCollisions();
  void updateCamera(const Player &player);
};

#endif /* GAME_HPP */