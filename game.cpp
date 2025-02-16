#include "game.hpp"
#include "camera.hpp"
#include "components/animationcomponent.hpp"
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
}

void Game::handleEvents() {}

void Game::update() {

  Entity &selectedPlayer = *(currentLevel.entities.at(selectedPlayerIndex));
  controlSystem.update(transformSystem, *selectedPlayer.GetComponent<TransformComponent>(), camera,
                       currentLevel);

  for (auto &entity : currentLevel.entities) {

    std::cout << "update loop start id " << entity->ID << " \n";
    collisionSystem.update(*entity, currentLevel);
    transformSystem.update(*entity->GetComponent<TransformComponent>());

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

    renderSystem.update(*entity);
  }

  updateCamera(*selectedPlayer.GetComponent<TransformComponent>());
}

void Game::render(SDL_Renderer &renderer) {
  std::cout << "render \n";
  SDL_RenderClear(&renderer);

  for (auto &object : currentLevel.entities) {
    renderSystem.render2(renderer, camera, *object->GetComponent<RenderComponent>());
  }
  SDL_RenderPresent(&renderer);
}

// TODO

void Game::updateCamera(TransformComponent &transformComponent) {
  if ((transformComponent.getXpos() < camera.m_position.x && transformComponent.getXvel() < 0) ||
      (transformComponent.getXpos() + PLAYER_WIDTH > camera.m_position.w &&
       transformComponent.getXvel() > 0)) {
    camera.m_offset.x += transformComponent.getXvel();
  }

  if ((transformComponent.getYpos() < camera.m_position.y && transformComponent.getYvel() < 0) ||
      (transformComponent.getYpos() + PLAYER_HEIGHT > camera.m_position.h &&
       transformComponent.getYvel() > 0)) {
    camera.m_offset.y += transformComponent.getYvel();
  }
  camera.updatePositionRelativeToScreenAndOffset();
}
