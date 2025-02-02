#include "game.hpp"
#include "camera.hpp"
#include "components/rendercomponent.hpp"
#include "entities/block.hpp"
#include "entities/player.hpp"
#include "physics.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

// TODO I think these should be defined else where because they are called as
// Game::something (don't like it)
SDL_Renderer *Game::renderer = nullptr;
TTF_Font *Game::gFont = nullptr;
SDL_Window *Game::window = nullptr;
// TODO System class?
void Game::init(const char *title, const int xpos, const int ypos, const int width,
                const int height, const bool fullscreen) {
  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    TTF_Init();
    gFont = TTF_OpenFont("font.ttf", 32);
    std::cout << "Subsystems initialised \n";
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window) {
      std::cout << "Window created\n";
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      // SDL_RenderSetScale(renderer, 2,2);
      std::cout << "Renderer created\n";
    }
    isRunning = true;
  } else {
    isRunning = false;
  }

  // TODO level data should be generated at the start of the game (main?), game obj should have
  // access to levels list
  SDL_RWops *Handle{SDL_RWFromFile("level", "rb")};
  currentLevel = level.createLevel(Handle);
  SDL_RWclose(Handle);

  selectedPlayer = currentLevel.playercharacters.at(0);
  std::cout << std::addressof(currentLevel.playercharacters.at(0)->renderComponent) << std::endl;
  std::cout << std::addressof(currentLevel.renderables.at(0)) << std::endl;
}

void Game::handleEvents() {
  SDL_Event event;

  // WaitForEvent pudottaa cpu% mutta tökkii
  if (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      // quit game
      case SDLK_q:
        isRunning = false;
        break;
      case SDLK_RIGHT:
        inputR = true;
        break;
      case SDLK_LEFT:
        inputL = true;
        break;
      // reset selectedPlayer pos for debugging
      case SDLK_r:
        selectedPlayer->transformComponent.setXvel(0);
        selectedPlayer->transformComponent.setYvel(0);
        selectedPlayer->transformComponent.setXpos(0);
        selectedPlayer->transformComponent.setYpos(0);
        selectedPlayer->setGravityState(INAIR);
        camera.m_position = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
        camera.m_offset = {0, 0, 0, 0};
        break;
      case SDLK_c:
        changePlayerCharacter();
        break;
      default:
        break;
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
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

void Game::update() {

  handleInput();

  checkPlayerMovementCollisions();

  // polymorphism is lost like this
  for (auto &object : currentLevel.playercharacters) {
    object->update();
  }

  // for (auto &object : currentLevel.blocks) {
  //   object->update();
  // }

  // for (auto &object : currentLevel.gameobjects) {
  //   object->update();
  // }

  updateCamera(*selectedPlayer);
}

void Game::render() {
  SDL_RenderClear(renderer);
  for (auto &object : currentLevel.renderables) {
    TextureManager::draw(object->render(), camera);
  }
  // for (auto &object : currentLevel.playercharacters) {
  //   object->render(camera);
  // }
  // for (auto &object : currentLevel.playercharacters) {
  //   object.render(camera);
  // }

  // for (auto &object : currentLevel.blocks) {
  //   object.render(camera);
  // }

  // for (auto &object : currentLevel.gameobjects) {
  //   object.render(camera);
  // }

  SDL_RenderPresent(renderer);
}

void Game::clean() {
  SDL_DestroyWindow(Game::window);
  SDL_DestroyRenderer(Game::renderer);
  TTF_CloseFont(Game::gFont);
  TTF_Quit();
  SDL_Quit();
  std::cout << "Game cleaned\n";
}

// TODO
void Game::changePlayerCharacter() {
  if (selectedPlayer->getName() == "Erik") {
    selectedPlayer->stopX();
    selectedPlayer = currentLevel.playercharacters.at(1);
    centerCameraToCharacter(*selectedPlayer);
    return;
  }

  if (selectedPlayer->getName() == "Olaf") {
    selectedPlayer->stopX();
    selectedPlayer = currentLevel.playercharacters.at(2);
    centerCameraToCharacter(*selectedPlayer);
    return;
  }

  if (selectedPlayer->getName() == "Baleog") {
    selectedPlayer->stopX();
    selectedPlayer = currentLevel.playercharacters.at(0);
    centerCameraToCharacter(*selectedPlayer);
  }
}

void Game::handleInput() const {
  if (inputR && !inputL) {
    selectedPlayer->moveRight();
    return;
  }

  if (inputL && !inputR) {
    selectedPlayer->moveLeft();
    return;
  }

  selectedPlayer->stopX();
}

void Game::checkPlayerMovementCollisions() {
  for (auto &&player : currentLevel.playercharacters) {

    player->collisionComponent.setCollisionDown(false);
    player->collisionComponent.setCollisionUp(false);
    player->collisionComponent.setCollisionRight(false);
    player->collisionComponent.setCollisionLeft(false);

    int yposDif = 0;
    int xposDif = 0;

    for (const auto &block : currentLevel.blocks) {

      // player comes from right
      if (Physics::playerCollisionLeft(*player, *block) == true) {
        xposDif =
            player->collisionComponent.collisionRect.x -
            (block->collisionComponent.collisionRect.x + block->collisionComponent.collisionRect.w);
        player->collisionComponent.setXposDifferenceWithCollisionObject(xposDif);
        player->collisionComponent.setCollisionLeft(true);
        continue;
      }

      // player comes from left
      if (Physics::playerCollisionRight(*player, *block) == true) {
        xposDif = (player->collisionComponent.collisionRect.x +
                   player->collisionComponent.collisionRect.w) -
                  block->collisionComponent.collisionRect.x;
        player->collisionComponent.setXposDifferenceWithCollisionObject(xposDif);
        player->collisionComponent.setCollisionRight(true);
        continue;
      }

      // player comes from above
      if (Physics::playerCollisionDown(*player, *block) == true) {
        yposDif = (player->collisionComponent.collisionRect.y +
                   player->collisionComponent.collisionRect.h) -
                  block->collisionComponent.collisionRect.y;
        player->collisionComponent.setYposDifferenceWithCollisionObject(yposDif);
        player->collisionComponent.setCollisionDown(true);

        continue;
      }

      // player comes from below
      if (Physics::playerCollisionUp(*player, *block) == true) {
        playerCollisionUp = true;
        continue;
      }
    }

    if (playerCollisionUp) {
      // TODO Fix when relevant
      player->transformComponent.setYpos(player->transformComponent.getYpos() -
                                         player->transformComponent.getYvel());
    }
  }
}

void Game::centerCameraToCharacter(const Player &player) {

  camera.updateOffsetRelativeToScreen(player.transformComponent.getXpos(),
                                      player.transformComponent.getYpos());
  camera.updatePositionRelativeToScreenAndOffset();
}

void Game::updateCamera(const Player &player) {
  if ((player.transformComponent.getXpos() < camera.m_position.x &&
       player.transformComponent.getXvel() < 0) ||
      (player.transformComponent.getXpos() + PLAYER_WIDTH > camera.m_position.w &&
       player.transformComponent.getXvel() > 0)) {
    camera.m_offset.x += player.transformComponent.getXvel();
  }

  if ((player.transformComponent.getYpos() < camera.m_position.y &&
       player.transformComponent.getYvel() < 0) ||
      (player.transformComponent.getYpos() + PLAYER_HEIGHT > camera.m_position.h &&
       player.transformComponent.getYvel() > 0)) {
    camera.m_offset.y += player.transformComponent.getYvel();
  }
  camera.updatePositionRelativeToScreenAndOffset();
}
