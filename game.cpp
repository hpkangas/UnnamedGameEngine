#include "game.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "entities/block.hpp"
#include "entities/player.hpp"
#include "gamesystem.hpp"
#include "physics.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <vector>

// TODO I think these should be defined else where because they are called as
// Game::something (don't like it)
// TODO System class?
void Game::init() {

  // TODO level data should be generated at the start of the game (main?), game obj should have
  // access to levels list
  SDL_RWops *Handle{SDL_RWFromFile("level", "rb")};
  currentLevel = level.createLevel(Handle);
  SDL_RWclose(Handle);

  selectedPlayer = currentLevel.playercharacters.at(0);
}

void Game::handleEvents() {}

void Game::update() {
  controlSystem.update(*selectedPlayer, camera, currentLevel);
  checkPlayerMovementCollisions();

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

void Game::render(SDL_Renderer &renderer) {
  renderSystem.render(renderer, camera, currentLevel.renderables);
}

// TODO

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
