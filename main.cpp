#include "game.hpp"
#include "constants.hpp"

Game *game = nullptr;

int main(int argc, char const *argv[])
{
    game = new Game();
     int ticks = SDL_GetTicks64();

    game->init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, false);

    while (game->running())
    {
        if (SDL_GetTicks64() - ticks < 1000/FRAMERATE)
        {
           continue;
        }
        ticks = SDL_GetTicks64();
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();               

    return 0;
}