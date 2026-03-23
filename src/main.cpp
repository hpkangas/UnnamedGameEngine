#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_main.h>
#include "utilities/constants.hpp"
#include "game.hpp"
#include "scene/scene.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  Game game;
  Scene scene;

  game.init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH,
            SCREENHEIGHT, false);

if (!scene.init()){
  std::cout<<"Error iniatializing scene. Abort. \n";
  game.clean();
  return 0;
}
  std::cout << "Initialization complete at " << SDL_GetTicks64() / 1000.0 << " seconds"
            << std::endl;

  while (game.running()) {
    Uint64 ticksStart = SDL_GetTicks64();
    scene.update();
    scene.render();
    Uint64 ticksEnd = SDL_GetTicks64() - ticksStart;
    if (FRAMEDELAY > ticksEnd) {
      SDL_Delay(FRAMEDELAY - ticksEnd);
      continue;
    }
  }

  std::cout << "End after " << SDL_GetTicks64() / 1000.0 << " seconds" << std::endl;
  game.clean();

  return 0;
}
