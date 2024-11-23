#include "game.hpp"
#include "gameobject.hpp"
#include "iostream"
#include <vector>
#include <list>
#include <stdlib.h>
#include "player.hpp"
#include "block.hpp"
#include "physics.cpp"

SDL_Renderer *Game::renderer = nullptr;
TTF_Font *Game::gFont = nullptr;

std::vector<GameObject *> objects;
std::vector<Block *> blocks;
Player *player = nullptr;
Block *block1 = nullptr;
Block *block2 = nullptr;
Block *block3 = nullptr;
Block *block4 = nullptr;
Block *block5 = nullptr;
Block *block6 = nullptr;

void Game::init(const char *title, int xpos, int ypos, int width, int HEIGHT, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        TTF_Init();
        gFont = TTF_OpenFont("font.ttf", 32);
        std::cout << "Subsystems initialised \n";
        window = SDL_CreateWindow(title, xpos, ypos, width, HEIGHT, flags);
        if (window)
        {
            std::cout << "Window created\n";
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // SDL_RenderSetScale(renderer, 2,2);
            std::cout << "Renderer created\n";
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    player = new Player("assets/baleog.png", 80, 80);
    GameText *text = new GameText("TEST TEXT", 200, 200);

    block1 = new Block("assets/tiles.png", 0, 0, 80, 428);
    block2 = new Block("assets/tiles.png", 0, 0, 80 + 128, 428);
    block3 = new Block("assets/tiles.png", 0, 0, 80 + 128 + 128, 488);
    block4 = new Block("assets/tiles.png", 0, 0, 80 + 128 + 128 + 128, 428);
    block5 = new Block("assets/tiles.png", 0, 0, 80 + 128 + 128 + 128 + 128 + 128 + 128, 428);
    block6 = new Block("assets/tiles.png", 0, 0, 80, 428 - 128);
    blocks.emplace_back(block1);
    blocks.emplace_back(block2);
    blocks.emplace_back(block3);
    blocks.emplace_back(block4);
    blocks.emplace_back(block5);
    blocks.emplace_back(block6);

    // objects.emplace_back(player);
    objects.emplace_back(block1);
    objects.emplace_back(block2);
    objects.emplace_back(block3);
    objects.emplace_back(block4);
    objects.emplace_back(block5);
    objects.emplace_back(block6);
    objects.emplace_back(text);

    // miten katsoa collision vain tietyille objekteille? Jos käytän listaa, miten nopeutan hakua?
}

void Game::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            // quit game
            case SDLK_q:
                isRunning = false;
            case SDLK_UP:
                player->moveUp();
                break;
            case SDLK_DOWN:
                player->moveDown();
                break;
            case SDLK_RIGHT:
                player->moveRight();
                break;
            case SDLK_LEFT:
                player->moveLeft();
                break;
            case SDLK_d:
                player->yvel = 80;
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                player->stopYU();
                break;
            case SDLK_DOWN:
                player->stopYD();
                break;
            case SDLK_RIGHT:
                player->stopXR();
                break;
            case SDLK_LEFT:
                player->stopXL();
                break;
            default:
                break;
            }
            break;
        }
    }
}

void Game::update()
{

    for (auto &block : blocks)
    {

        // player comes from right
        if (Physics::playerCollisionLeft(*player, *block) == true)
        {
            // tää pitäisi pystyä tekemään siistimmin ja modulaarisemmin
            // älä säädä pelaajan xposia tässä vaan playerin metodissa
            // tai anna uusi arvo playerille, mitä käytetään player->updatessa
            // player->xvel = 0;
            // player->moveToX(block->collisionRect.x + block->collisionRect.w);
            player->xpos -= player->xvel;
            player->animationState = PUSHING;
        }

        // player comes from left
        if (Physics::playerCollisionRight(*player, *block) == true)
        {
            player->xpos -= player->xvel;
            player->animationState = PUSHING;
        }

        // player comes from above

        if (Physics::playerCollisionDown(*player, *block) == true)
        {

            player->ypos -= player->yvel;
            player->canJump = true;
        }

        // player comes from below
        if (Physics::playerCollisionUp(*player, *block) == true)
        {

            player->ypos -= player->yvel;
        }
    }

    for (auto &object : objects)
    {
        object->update();
    }
    player->update();
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &object : objects)
    {
        object->render();
    }
    player->render();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(gFont);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Game cleaned\n";
}

int Game::getCurrentTime()
{
    // get time elapsed since game was launched, required for timing
    currentTime = SDL_GetTicks();
    return currentTime;
}
