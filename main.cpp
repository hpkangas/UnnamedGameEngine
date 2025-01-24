#include "constants.hpp"
#include "game.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  const auto game = new Game();

  Uint64 ticks = 0;

  game->init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             SCREENWIDTH, SCREENHEIGHT, false);
  // SDL_Delay mukaan?
  //  auttaa tehojen syömiseen mutta vähän tahmea ehkä. ei responsiivinen jos
  //  odottaa ennen kuin liikkuu
  // vaatii työstöä
  while (game->running()) {
    if (SDL_GetTicks64() - ticks < 1000.0f / FRAMERATE) {
      // printf("Delay %d\n", SDL_GetTicks64() - ticks);
      continue;
    }
    std::cout<<"asd asd";
    game->handleEvents();
    std::cout<<"asd asd";
    game->update();
    std::cout<<"asd asd";
    game->render();
    std::cout<<"asd asd";
    ticks = SDL_GetTicks64();
    // SDL_Delay(SDL_GetTicks64() - ticks);
  }

  Game::clean();

  return 0;
}