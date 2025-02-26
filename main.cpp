#include "SDL_timer.h"
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
  std::cout << "Initialization complete at " << SDL_GetTicks64() / 1000.0 << " seconds" << std::endl;

  while (system.running()) {
    Uint64 ticksStart = SDL_GetTicks64();
    game.update();
    game.render(*system.renderer);
    Uint64 ticksEnd = SDL_GetTicks64() - ticksStart;
    if (FRAMEDELAY > ticksEnd) {
      SDL_Delay(FRAMEDELAY - ticksEnd);
      continue;
    }
  }
  std::cout << "End after "<< SDL_GetTicks64() / 1000.0 << " seconds" << std::endl;
  system.clean();

  return 0;
}
