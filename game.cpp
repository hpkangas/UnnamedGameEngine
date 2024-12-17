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
Block *block7 = nullptr;
Block *block8 = nullptr;
Block *block9 = nullptr;
Block *block10 = nullptr;

// bool keyPressed = false;
// bool keyLetGo = false;
// bool inputU = false;
// bool inputD = false;
bool inputR = false;
bool inputL = false;
// std::vector<bool> keys;

void Game::init(const char *title, int xpos, int ypos, int width, int HEIGHT, bool fullscreen)
{

    // keys.emplace_back(inputU);
    // keys.emplace_back(inputD);
    // keys.emplace_back(inputR);
    // keys.emplace_back(inputL);

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
    block2 = new Block("assets/tiles.png", 128, 0, 80 + 128, 428);
    block3 = new Block("assets/tiles.png", 0, 0, 80 + 128 + 128, 488);
    block4 = new Block("assets/tiles.png", 128, 0, 80 + 128 + 128 + 128, 428);
    block5 = new Block("assets/tiles.png", 0, 0, 80 + 128 + 128 + 128 + 128 + 128 + 128, 428);
    block7 = new Block("assets/tiles.png", 128, 0, 80 + 128 + 128 + 128 + 128 + 128 + 128 + 128, 428);
    block8 = new Block("assets/tiles.png", 256, 0, 80 + 128 + 128 + 128 + 128 + 128 + 128 + 128 + 128, 428);
    block9 = new Block("assets/tiles.png", 256, 0, 128, 64, 80 + 128 + 128 + 128 + 128 + 128 + 128 + 128 + 128 + 128, 628);
    block6 = new Block("assets/tiles.png", 0, 0, 80, 428 - 128);
    block10 = new Block("assets/tiles.png", 128, 0, 128 * 4, 428 - 64);
    blocks.emplace_back(block1);
    blocks.emplace_back(block2);
    blocks.emplace_back(block3);
    blocks.emplace_back(block4);
    blocks.emplace_back(block5);
    blocks.emplace_back(block6);
    blocks.emplace_back(block7);
    blocks.emplace_back(block8);
    blocks.emplace_back(block9);
    blocks.emplace_back(block10);

    objects.emplace_back(player);
    objects.emplace_back(block1);
    objects.emplace_back(block2);
    objects.emplace_back(block3);
    objects.emplace_back(block4);
    objects.emplace_back(block5);
    objects.emplace_back(block6);
    objects.emplace_back(block7);
    objects.emplace_back(block8);
    objects.emplace_back(block9);
    objects.emplace_back(block10);
    objects.emplace_back(text);
}

void Game::handleEvents()
{
    SDL_Event event;

    // WaitForEvent pudottaa cpu% mutta tökkii
    if (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            // quit game
            case SDLK_q:
                isRunning = false;
                break;
            case SDLK_UP:

                player->moveUp();
                break;
            case SDLK_DOWN:
                player->moveDown();
                break;
            case SDLK_RIGHT:
                //  keyPressed = true;
                inputR = true;
                // inputR = true;
                break;
            case SDLK_LEFT:
                // keyPressed = true;
                inputL = true;
                // inputL = true;
                break;
            case SDLK_d:
                player->yvel = 80;
                break;
            // reset player pos
            case SDLK_r:
                player->xvel = 0;
                player->yvel = 0;
                player->xpos = 0;
                player->ypos = 0;
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
                // keyPressed = true;
                inputR = false;
                break;
            case SDLK_LEFT:
                // keyPressed = true;
                inputL = false;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void Game::update()
{

    playerCollisionDown = false;
    playerCollisionUp = false;
    playerCollisionLeft = false;
    playerCollisionRight = false;

    {

        if (inputR && !inputL)
        {
            player->moveRight();
        }

        if (inputL && !inputR)
        {
            player->moveLeft();
        }

        if (!inputL && !inputR || inputL && inputR)
        {
            player->stopX();
        }
    }

    for (auto &block : blocks)
    {

        // player comes from right
        if (Physics::playerCollisionLeft(*player, *block) == true)
        {
            playerCollisionLeft = true;
            continue;
        }

        // player comes from left
        if (Physics::playerCollisionRight(*player, *block) == true)
        {
            playerCollisionRight = true;
            continue;
        }

        // player comes from above
        if (Physics::playerCollisionDown(*player, *block) == true)
        {
            playerCollisionDown = true;
            continue;
        }

        // player comes from below
        if (Physics::playerCollisionUp(*player, *block) == true)
        {
            playerCollisionUp = true;
            continue;
        }
    }

    if (playerCollisionDown)
    {
        player->yvel = 0.0f;
        player->canJump = true;
        player->gravityState = GROUNDED;
        if (player->animationState == anims_INAIR)
            player->animationState = STILL;
    }
    else
    {
        player->gravityState = INAIR;
        player->animationState = anims_INAIR;
    }

    if (playerCollisionLeft)
    {
        player->xpos -= player->xvel;
        if (player->gravityState == GROUNDED)
            player->animationState = PUSHING;
    }

    if (playerCollisionRight)
    {
        player->xpos -= player->xvel;
        if (player->gravityState == GROUNDED)
            player->animationState = PUSHING;
    }

    if (playerCollisionUp)
    {
        playerCollisionUp = true;
        player->ypos -= player->yvel;
    }

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
