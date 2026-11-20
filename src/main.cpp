#include "scene/scene.hpp"
#include "system.hpp"
#include "texturemanager.hpp"
#include "utilities/constants.hpp"
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_timer.h>
#include <iostream>

int main(int argc, char const *argv[]) {
  System system;

  system.init("My Cool Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
              Constants::SCREENWIDTH, Constants::SCREENHEIGHT, false);

  while (system.running()) {

    Scene scene;
    TextureManager::loadTextures();
    if (!scene.init()) {
      std::cout << "Error iniatializing scene. Abort. \n";
      system.clean();
      return 0;
    }

    std::cout << "Initialization complete at " << SDL_GetTicks64() / 1000.0 << " seconds"
              << std::endl;

    auto nextFrame = static_cast<double>(SDL_GetTicks64());

    while (Scene::isRunning) {
      scene.update();
      scene.render();

      nextFrame += Constants::FRAME_MS;
      const auto now = static_cast<double>(SDL_GetTicks64());

      if (now < nextFrame) {
        SDL_Delay(static_cast<Uint32>(nextFrame - now));
      } else if (now - nextFrame > Constants::MAX_LAG_MS) {
        nextFrame = now;
      }
    }
    TextureManager::clean();
  }

  std::cout << "End after " << SDL_GetTicks64() / 1000.0 << " seconds" << std::endl;
  system.clean();

  return 0;
}
