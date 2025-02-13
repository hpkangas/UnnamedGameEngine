#include "game.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "entities/player.hpp"
#include "gamesystem.hpp"
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
  controlSystem.update(transformSystem, *selectedPlayer, camera, currentLevel);
  for (auto &object : currentLevel.playercharacters) {
    collisionSystem.update(*object, currentLevel);

    transformSystem.update(*object);
    gravitySystem.update(*object, object->collisionComponent, object->gravityComponent,
                         object->animationComponent, object->transformComponent);
    animationSystem.update(object->collisionComponent, object->gravityComponent.getGravityState(),
                           object->transformComponent, object->renderComponent,
                           object->animationComponent);
    renderSystem.update(*object);
    // original update func
    //  object->update();
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
