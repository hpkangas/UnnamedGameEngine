#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "camera.hpp"
#include "entities/player.hpp"
#include "level.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Game {
public:
  void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

  void handleEvents();
  void update();
  void render();
  static void clean();

  bool running() const { return isRunning; }

  static SDL_Renderer *renderer;
  static TTF_Font *gFont;

private:
  Player *selectedPlayer = nullptr;
  Level level;
  LevelInfo currentLevel;
  Camera camera;

  bool isRunning = false;

  bool inputR = false;
  bool inputL = false;

  bool playerCollisionUp = false;

  static SDL_Window *window;

  void handleInput() const;
  void checkPlayerMovementCollisions();
  void changePlayerCharacter();
  void centerCameraToCharacter(const Player &player);
  void updateCamera(const Player &player);
};

#endif /* GAME_HPP */