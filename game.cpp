#include "game.hpp"
#include "gameobject.hpp"
#include "iostream"
#include <vector>
#include <list>
#include <stdlib.h>
#include "player.hpp"

SDL_Renderer *Game::renderer = nullptr;
TTF_Font *Game::gFont = nullptr;

std::list<GameObject *> objects;
Player *player = nullptr;

Game::Game()
{
}

Game::~Game()
{
}

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

    player = new Player("assets/baleog.png", 80, 280);

    objects.emplace_back(player);
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
            case SDLK_q:
                isRunning = false;
            case SDLK_UP:
                player->yvel = -4;
                break;
            case SDLK_DOWN:
                player->yvel = 4;
                break;
            case SDLK_RIGHT:
                player->xvel = 4;
                 player->state = WALKING;
                break;
            case SDLK_LEFT:
                player->xvel = -4;
                player->state = WALKING;
            break;
            
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                if (player->yvel < 0)
                    player->yvel = 0;
                break;
            case SDLK_DOWN:
                if (player->yvel > 0)
                    player->yvel = 0;
                break;
            case SDLK_RIGHT:
                if (player->xvel > 0)
                    player->xvel = 0;
                    player->state = STILL;
                break;
            case SDLK_LEFT:
                if (player->xvel < 0)
                    player->xvel = 0;
                    player->state = STILL;
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
    for (auto &object : objects)
    {
        object->update();
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &object : objects)
    {
        object->render();
    }

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
