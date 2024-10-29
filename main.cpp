#include "game.hpp"
#include "constants.hpp"

Game *game = nullptr;

int main(int argc, char const *argv[])
{
    game = new Game();

    game->init("Totally not Lost Vikings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, false);

    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();               

    return 0;
}