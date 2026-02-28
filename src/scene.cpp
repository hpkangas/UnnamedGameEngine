#include "scene.hpp"
#include "camera.hpp"
#include "components/gravitycomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "systems/transformsystem.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>

void Scene::init() {
  SDL_RWops *Handle{SDL_RWFromFile("src/level", "rb")};
  currentLevel = SceneCreator::createLevel(Handle);
  SDL_RWclose(Handle);
  std::cout << "Scene init \n";
}

void Scene::update() {
  // TODO Some wonkiness with collision sprite position and walking animation staying after changing
  // character
  std::cout << "--------------" << std::endl;

  if (selectedPlayer != selectedPlayerIndex) {
    selectedPlayer = selectedPlayerIndex;
    centerCamera(*currentLevel.tcs.at(selectedPlayer));
  };

  // What if component list doens't have the id/i? How to go through only ids that are in the list?
  for (int id : currentLevel.gravityIds) {
    gravitySystem.update(*currentLevel.ctrlComponents.at(id), *currentLevel.gcs.at(id),
                         *currentLevel.tcs.at(id));
  }

  // for (int id : currentLevel.collisionIds) {
  collisionDetectionSystem.update(0, currentLevel);
  // }

  // Not all collision have controls too, will crash with id for range loop
  // for (int i = 0; i < 3; i++) {
  collisionSystem.update(0, currentLevel, transformSystem);
  // }
  for (int id : currentLevel.controlIds) {
    controlSystem.update(transformSystem, *currentLevel.tcs.at(id), id, selectedPlayerIndex,
                         *currentLevel.ctrlComponents.at(id), *currentLevel.gcs.at(id),
                         *currentLevel.ccs.at(id));
  }

  for (int id : currentLevel.transformIds) {
    transformSystem.update(*currentLevel.tcs.at(id));
  }

  for (int id : currentLevel.animationIds) {
    animationSystem.update(*currentLevel.ctrlComponents.at(id), *currentLevel.ccs.at(id),
                           currentLevel.gcs.at(id)->getGravityState(), *currentLevel.tcs.at(id),
                           *currentLevel.rcs.at(id), *currentLevel.acs.at(id));
  }
  for (int id : currentLevel.renderIds) {
    renderSystem.update(*currentLevel.tcs.at(id), *currentLevel.rcs.at(id));
  }

  updateCamera(*currentLevel.tcs.at(selectedPlayer));
}

void Scene::render() { renderSystem.renderComponents(currentLevel.rcs); }

void Scene::updateCamera(TransformComponent &transformComponent) {
  if ((transformComponent.getXpos() < Camera::position.x && transformComponent.getXvel() < 0) ||
      (transformComponent.getXpos() + PLAYER_WIDTH > Camera::position.w &&
       transformComponent.getXvel() > 0)) {
    Camera::offset.x += transformComponent.getXvel();
  }

  if ((transformComponent.getYpos() < Camera::position.y && transformComponent.getYvel() < 0) ||
      (transformComponent.getYpos() + PLAYER_HEIGHT > Camera::position.h &&
       transformComponent.getYvel() > 0)) {
    Camera::offset.y += transformComponent.getYvel();
  }
  Camera::updatePositionRelativeToScreenAndOffset();
}

void Scene::centerCamera(TransformComponent &transformComponent) {
  Camera::updateOffsetRelativeToScreen(transformComponent.getXpos(), transformComponent.getYpos());
}
