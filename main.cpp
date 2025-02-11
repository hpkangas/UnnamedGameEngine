#include "constants.hpp"
#include "game.hpp"
#include "gamesystem.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  Game game;
  GameSystem system;

  Uint64 ticks = 0;

  system.init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
              SCREENWIDTH, SCREENHEIGHT, false);
  game.init();
  // SDL_Delay mukaan?
  //  auttaa tehojen syömiseen mutta vähän tahmea ehkä. ei responsiivinen jos
  //  odottaa ennen kuin liikkuu
  // vaatii työstöä

  std::cout << "init complete" << std::endl;
  while (system.running()) {
    if (SDL_GetTicks64() - ticks < 1000.0f / FRAMERATE) {
      // printf("Delay %d\n", SDL_GetTicks64() - ticks);
      continue;
    }
    //game.handleEvents();
    game.update();
    game.render(*system.renderer);
    ticks = SDL_GetTicks64();
    // SDL_Delay(SDL_GetTicks64() - ticks);
  }
  std::cout << "end" << std::endl;

  system.clean();

  return 0;
}