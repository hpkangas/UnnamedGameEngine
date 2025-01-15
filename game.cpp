#include "game.hpp"
#include "gameobject.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include "player.hpp"
#include "block.hpp"
#include "physics.cpp"
#include "camera.hpp"

SDL_Renderer *Game::renderer = nullptr;
TTF_Font *Game::gFont = nullptr;

std::vector<GameObject *> objects;
std::vector<Block *> blocks;
Player *player1 = nullptr;
Player *player2 = nullptr;
Player *player3 = nullptr;
std::vector<Player *> characters;
Player *selectedPlayer = nullptr;

Block *block01 = nullptr;
Block *block0 = nullptr;
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

bool inputR = false;
bool inputL = false;

bool playerCollisionUp = false;

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

    Camera camera;

    player1 = new Erik("assets/baleog.png", 80, 80, "Erik");
    player2 = new Olaf("assets/baleog.png", 180, 80, "Olaf");
    player3 = new Baelog("assets/baleog.png", 280, 80, "Baelog");
    selectedPlayer = player1;

    GameText *text = new GameText("TEST TEXT", 200, 200);

    block01 = new Block("assets/tiles.png", 0, 0, -128, 428);
    block0 = new Block("assets/tiles.png", 0, 0, 0, 428);
    block1 = new Block("assets/tiles.png", 0, 0, 128, 428);
    block2 = new Block("assets/tiles.png", 128, 0, 128 * 2, 428);
    block3 = new Block("assets/tiles.png", 256, 0, 128 * 3, 428);

    block4 = new Block("assets/tiles.png", 0, 0, 128 * 4, 428 + 128);
    block5 = new Block("assets/tiles.png", 128, 0, 128 * 5, 428 + 128);
    block7 = new Block("assets/tiles.png", 256, 0, 128 * 6, 400);

    block8 = new Block("assets/tiles.png", 0, 0, 128 * 7, 428 + 128 * 2);
    block9 = new Block("assets/tiles.png", 128, 0, 128 * 8, 428 + 128 * 2);
    block6 = new Block("assets/tiles.png", 256, 0, 128 * 9, 428 + 128 * 2);

    block10 = new Block("assets/tiles.png", 128, 0, 128 * 12, 428 + 128 * 4);
    blocks.emplace_back(block01);
    blocks.emplace_back(block0);
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

    objects.emplace_back(player1);
    objects.emplace_back(player2);
    objects.emplace_back(player3);

    objects.emplace_back(block01);
    objects.emplace_back(block0);
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

    characters.emplace_back(player1);
    characters.emplace_back(player2);
    characters.emplace_back(player3);

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

                selectedPlayer->moveUp();
                break;
            case SDLK_DOWN:
                selectedPlayer->moveDown();
                break;
            case SDLK_RIGHT:
                inputR = true;
                break;
            case SDLK_LEFT:
                inputL = true;
                break;
            case SDLK_d:
                selectedPlayer->setYvel(80);
                break;
            // reset selectedPlayer pos for debugging
            case SDLK_r:
                selectedPlayer->setXvel(0);
                selectedPlayer->setYvel(0);
                selectedPlayer->setXpos(0);
                selectedPlayer->setYpos(0);
                selectedPlayer->setGravityState(INAIR);
                camera.setPos({SCREENWIDTH * 1 / 4, 0, SCREENWIDTH * 3 / 4, 0});
                camera.setOffset({0, 0, 0, 0});
                break;
            case SDLK_c:
                changePlayerCharacter();
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                selectedPlayer->stopYU();
                break;
            case SDLK_DOWN:
                selectedPlayer->stopYD();
                break;
            case SDLK_RIGHT:
                inputR = false;
                break;
            case SDLK_LEFT:
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

    handleInput();


    checkPlayerMovementCollisions();

    for (auto &object : objects)
    {
        object->update();
    }
    
    updateCamera(selectedPlayer);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &object : objects)
    {
        object->render(camera);
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

void Game::changePlayerCharacter()
{
    if (selectedPlayer->name == "Erik")
    {
        selectedPlayer->stopX();
        selectedPlayer = player2;
        centerCameraToCharacter(selectedPlayer);
        return;
    }

    if (selectedPlayer->name == "Olaf")
    {
        selectedPlayer->stopX();
        selectedPlayer = player3;
        centerCameraToCharacter(selectedPlayer);
        return;
    }

    if (selectedPlayer->name == "Baelog")
    {
        selectedPlayer->stopX();
        selectedPlayer = player1;
        centerCameraToCharacter(selectedPlayer);
        return;
    }
}

int Game::getCurrentTime()
{
    // get time elapsed since game was launched, required for timing
    currentTime = SDL_GetTicks();
    return currentTime;
}

void Game::checkPlayerMovementCollisions()
{
    for (auto &&player : characters)
    {

        player->setCollisionDown(false);
        player->setCollisionUp(false);
        player->setCollisionRight(false);
        player->setCollisionLeft(false);

        int yposDif = 0;
        int xposDif = 0;

        for (auto &block : blocks)
        {

            // player comes from right
            if (Physics::playerCollisionLeft(*player, *block) == true)
            {
                xposDif = player->collisionRect.x - (block->collisionRect.x + block->collisionRect.w);
                player->setXposDifferenceWithCollisionObject(xposDif);
                player->setCollisionLeft(true);
                continue;
            }

            // player comes from left
            if (Physics::playerCollisionRight(*player, *block) == true)
            {
                xposDif = (player->collisionRect.x + player->collisionRect.w) - block->collisionRect.x;
                player->setXposDifferenceWithCollisionObject(xposDif);
                player->setCollisionRight(true);
                continue;
            }

            // player comes from above
            if (Physics::playerCollisionDown(*player, *block) == true)
            {
                yposDif = (player->collisionRect.y + player->collisionRect.h) - block->collisionRect.y;
                player->setYposDifferenceWithCollisionObject(yposDif);
                player->setCollisionDown(true);

                continue;
            }

            // player comes from below
            if (Physics::playerCollisionUp(*player, *block) == true)
            {
                playerCollisionUp = true;
                continue;
            }
        }

        if (playerCollisionUp)
        {
            // TODO Fix when relevant
            player->setYpos(player->getYpos() - player->getYvel());
        }
    }
}

void Game::handleInput()
{
    if (inputR && !inputL)
    {
        selectedPlayer->moveRight();
    }

    if (inputL && !inputR)
    {
        selectedPlayer->moveLeft();
    }

    if (!inputL && !inputR || inputL && inputR)
    {
        selectedPlayer->stopX();
    }
}

void Game::centerCameraToCharacter(Player *player)
{
   
    camera.offset.x = player->getXpos() - SCREENWIDTH * 1 / 2;
    camera.offset.y = player->getYpos() - SCREENHEIGHT * 1 / 2;

    camera.updateCameraPosition();
}

void Game::updateCamera(Player *player)
{
    if (player->getXpos() < camera.position.x && player->getXvel() < 0 ||
        player->getXpos() + PLAYER_WIDTH > camera.position.w && player->getXvel() > 0)
    {
        camera.offset.x += player->getXvel();
    }

    if (player->getYpos() < camera.position.y && player->getYvel() < 0 ||
        player->getYpos() + PLAYER_HEIGHT > camera.position.h && player->getYvel() > 0)
    {
        camera.offset.y += player->getYvel();
    }
    camera.updateCameraPosition();
}
