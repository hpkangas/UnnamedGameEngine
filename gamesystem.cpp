#include "gamesystem.hpp"
#include "SDL_image.h"
#include <iostream>

void GameSystem::init(const char *title, const int xpos, const int ypos, const int width,
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
  } else {
    isRunning = false;
  }
}

void GameSystem::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  TTF_CloseFont(gFont);
  TTF_Quit();
  SDL_Quit();
  std::cout << "Game cleaned\n";
}

bool GameSystem::running() { return isRunning; }

SDL_Texture *LoadTexture(SDL_Renderer &renderer, const char *fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName);
  if (tempSurface != nullptr) {
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(&renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
  }
  std::cout << "ERROR LOADING TEXTURE " << fileName << std::endl;
  return nullptr;
}

std::vector<SDL_Texture *> loadTextures(SDL_Renderer &renderer) {
  std::vector<SDL_Texture *> textures;
  textures.emplace_back(LoadTexture(renderer, "../assets/tiles.png"));
  textures.emplace_back(LoadTexture(renderer, "../assets/baleog.png"));
  return textures;
};