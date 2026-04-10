#include "scene/scene.hpp"
#include "system.hpp"
#include "utilities/constants.hpp"
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_timer.h>
#include <iostream>

int main(int argc, char const *argv[]) {
  System system;

  system.init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
              Constants::SCREENWIDTH, Constants::SCREENHEIGHT, false);

  while (system.running()) {

    Scene scene;
    if (!scene.init()) {
      std::cout << "Error iniatializing scene. Abort. \n";
      system.clean();
      return 0;
    }

    std::cout << "Initialization complete at " << SDL_GetTicks64() / 1000.0 << " seconds"
              << std::endl;

    while (Scene::isRunning) {
      Uint64 ticksStart = SDL_GetTicks64();
      scene.update();
      scene.render();
      Uint64 ticksEnd = SDL_GetTicks64() - ticksStart;
      if (Constants::FRAMEDELAY > ticksEnd) {
        SDL_Delay(Constants::FRAMEDELAY - ticksEnd);
        continue;
      }
    }
  }

  std::cout << "End after " << SDL_GetTicks64() / 1000.0 << " seconds" << std::endl;
  system.clean();

  return 0;
}
