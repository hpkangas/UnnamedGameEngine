#include "game.hpp"
#include "camera.hpp"
#include "components/collisioncomponent.hpp"
#include "components/gravitycomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
#include "gamesystem.hpp"
#include "systems/transformsystem.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>

void Game::init() {

  SDL_RWops *Handle{
      SDL_RWFromFile("/home/hannu-pekkakangas/Projects/notlostvikings/src/level", "rb")};
  currentLevel = level.createLevel(Handle);
  SDL_RWclose(Handle);
  std::cout << "Game init \n";

  // renderSystem.components = currentLevel.rcs;
  // for (auto &entity : currentLevel.rcs) {
  //   std::cout << entity.second.render().destinationRect.x << "\n";
  //   // tcs.emplace_back(std::move(GetComponent<TransformComponent>()));
  //   // ccs.emplace_back(std::move(GetComponent<CollisionComponent>()));
  //   // gcs.emplace_back(std::move(GetComponent<GravityComponent>()));
  //   // acs.emplace_back(std::move(GetComponent<AnimationComponent>()));
  // }
}

void Game::update() {
  // Entity &selectedPlayer = *(currentLevel.entities.at(selectedPlayerIndex));
  controlSystem.update(transformSystem, *currentLevel.tcs.at(selectedPlayer), camera,
                       selectedPlayerIndex);

  if (selectedPlayer != selectedPlayerIndex) {
    selectedPlayer = selectedPlayerIndex;
    centerCamera(*currentLevel.tcs.at(selectedPlayer));
  };

  for (auto &entity : currentLevel.entities) {
    collisionSystem.update(*entity, currentLevel);
  }

  for (int i = 0; i < currentLevel.gcs.size(); i++) {
    gravitySystem.update(currentLevel.ccs.at(i)->getCollisionDown(), *currentLevel.gcs.at(i),
                         *currentLevel.tcs.at(i));
  }

  for (int i = 0; i < currentLevel.tcs.size(); i++) {
    transformSystem.update(*currentLevel.tcs.at(i));
  }

  for (int i = 0; i < currentLevel.rcs.size(); i++) {
    renderSystem.update(*currentLevel.tcs.at(i), *currentLevel.rcs.at(i));
  }

  for (int i = 0; i < currentLevel.acs.size(); i++) {

    animationSystem.update(*currentLevel.ccs.at(i), currentLevel.gcs.at(i)->getGravityState(),
                           *currentLevel.tcs.at(i), *currentLevel.rcs.at(i),
                           *currentLevel.acs.at(i));
  }

  updateCamera(*currentLevel.tcs.at(selectedPlayer));
}

void Game::render() {
  // SDL_RenderClear(GameSystem::renderer);

  // for (auto &object : currentLevel.entities) {
  //   renderSystem.render(*object->GetComponent<RenderComponent>());
  // }
  // SDL_RenderPresent(GameSystem::renderer);

  renderSystem.renderComponents(currentLevel.rcs);
}

void Game::updateCamera(TransformComponent &transformComponent) {
  if ((transformComponent.getXpos() < Camera::m_position.x && transformComponent.getXvel() < 0) ||
      (transformComponent.getXpos() + PLAYER_WIDTH > Camera::m_position.w &&
       transformComponent.getXvel() > 0)) {
    Camera::m_offset.x += transformComponent.getXvel();
  }

  if ((transformComponent.getYpos() < Camera::m_position.y && transformComponent.getYvel() < 0) ||
      (transformComponent.getYpos() + PLAYER_HEIGHT > Camera::m_position.h &&
       transformComponent.getYvel() > 0)) {
    Camera::m_offset.y += transformComponent.getYvel();
  }
  Camera::updatePositionRelativeToScreenAndOffset();
}

void Game::centerCamera(TransformComponent &transformComponent) {
  Camera::updateOffsetRelativeToScreen(transformComponent.getXpos(), transformComponent.getYpos());
}
