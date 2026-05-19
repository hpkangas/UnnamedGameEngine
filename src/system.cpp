#include "system.hpp"
#include "SDL_ttf.h"
#include "texturemanager.hpp"
#include <cstddef>
#include <iostream>

void System::init(const char *title, const int xpos, const int ypos, const int width,
                  const int height, const bool fullscreen) const {
  isRunning = true;

  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    // if (!TTF_Init()) {
    //   std::cout << TTF_GetError() << std::endl;
    //   isRunning = false;
    // }

    // gFont = TTF_OpenFont("font.ttf", 32);
    // if (gFont == nullptr) {
    //   std::cout << TTF_GetError() << std::endl;
    //   isRunning = false;
    // }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
      std::cout << SDL_GetError() << std::endl;
      isRunning = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
      std::cout << SDL_GetError() << std::endl;
      isRunning = false;
    } else {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
  }
}

void System::clean() const {
  TTF_CloseFont(gFont);
  gFont = nullptr;

  TextureManager::clean();

  SDL_DestroyRenderer(renderer);
  renderer = nullptr;

  SDL_DestroyWindow(window);
  window = nullptr;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  std::cout << "Game cleaned\n";
}

bool System::running() const { return isRunning; }
