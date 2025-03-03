#include "game.hpp"
#include <iostream>

void Game::init(const char *title, const int xpos, const int ypos, const int width,
                const int height, const bool fullscreen) {

  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    TTF_Init();
    gFont = TTF_OpenFont("font.ttf", 32);
    std::cout << "Subsystems initialised \n";
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window) {
      std::cout << "Window created\n";
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      // SDL_RenderSetScale(renderer, 2,2);
      std::cout << "Renderer created\n";
    }
    isRunning = true;
    std::cout << "System init \n";
  } else {
    isRunning = false;
  }
}

void Game::clean() {
  TTF_CloseFont(gFont);
  TTF_Quit();
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  std::cout << "Game cleaned\n";
}

bool Game::running() { return isRunning; }
