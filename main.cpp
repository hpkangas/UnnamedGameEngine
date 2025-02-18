#include "constants.hpp"
#include "game.hpp"
#include "gamesystem.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  GameSystem system;
  Game game;

  system.init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
              SCREENWIDTH, SCREENHEIGHT, false);
  game.init();
  // SDL_Delay mukaan?
  //  auttaa tehojen syömiseen mutta vähän tahmea ehkä. ei responsiivinen jos
  //  odottaa ennen kuin liikkuu
  // vaatii työstöä

  std::cout << "init complete" << std::endl;
  while (system.running()) {
    Uint64 tickstart = SDL_GetTicks64();
    // game.handleEvents();
    game.update();
    game.render(*system.renderer);
    Uint64 ticks = SDL_GetTicks64() - tickstart;
    if (FRAMEDELAY > ticks) {
      SDL_Delay(FRAMEDELAY - ticks);
      continue;
    }
  }
  std::cout << "end" << std::endl;
  system.clean();

  return 0;
}
