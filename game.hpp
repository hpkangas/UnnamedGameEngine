#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "camera.hpp"
#include "player.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
public:
  void init(const char *title, int xpos, int ypos, int width, int heigth, bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();

  bool running() { return isRunning; }

  static SDL_Renderer *renderer;
  static TTF_Font *gFont;

private:
  bool isRunning;
  static SDL_Window *window;
  Camera camera;

  bool inputR = false;
  bool inputL = false;

  bool playerCollisionUp = false;

  void handleInput();
  void checkPlayerMovementCollisions();
  void changePlayerCharacter();
  void centerCameraToCharacter(Player *player);
  void updateCamera(Player *player);
};

#endif /* GAME_HPP */