#include "game.hpp"
#include "camera.hpp"
#include "components/animationcomponent.hpp"
#include "components/collisioncomponent.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
#include "entities/entity.hpp"
#include "gamesystem.hpp"
#include "systems/transformsystem.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>

void Game::init() {

  SDL_RWops *Handle{SDL_RWFromFile("level", "rb")};
  currentLevel = level.createLevel(Handle);
  SDL_RWclose(Handle);
  std::cout << "Game init \n";
}

void Game::update() {

  Entity &selectedPlayer = *(currentLevel.entities.at(selectedPlayerIndex));
  int currentplayer = selectedPlayerIndex;
  controlSystem.update(transformSystem, *selectedPlayer.GetComponent<TransformComponent>(), camera,
                       selectedPlayerIndex);

  if (currentplayer != selectedPlayerIndex) {
    Entity &selectedPlayer = *(currentLevel.entities.at(selectedPlayerIndex));
    centerCamera(*selectedPlayer.GetComponent<TransformComponent>());
  };

  for (auto &entity : currentLevel.entities) {

    collisionSystem.update(*entity, currentLevel);

    if (entity->GetComponent<CollisionComponent>() != nullptr &&
        entity->GetComponent<GravityComponent>() != nullptr) {

      gravitySystem.update(*entity->GetComponent<CollisionComponent>(),
                           *entity->GetComponent<GravityComponent>(),
                           *entity->GetComponent<TransformComponent>());
    }

    if (entity->GetComponent<AnimationComponent>() != nullptr) {

      animationSystem.update(*entity->GetComponent<CollisionComponent>(),
                             entity->GetComponent<GravityComponent>()->getGravityState(),
                             *entity->GetComponent<TransformComponent>(),
                             *entity->GetComponent<RenderComponent>(),
                             *entity->GetComponent<AnimationComponent>());
    }

    transformSystem.update(*entity->GetComponent<TransformComponent>());
    renderSystem.update(*entity);
  }

  updateCamera(*selectedPlayer.GetComponent<TransformComponent>());
}

void Game::render() {
  SDL_RenderClear(GameSystem::renderer);

  for (auto &object : currentLevel.entities) {
    renderSystem.render(*object->GetComponent<RenderComponent>());
  }
  SDL_RenderPresent(GameSystem::renderer);
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
