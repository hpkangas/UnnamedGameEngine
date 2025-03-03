#include "scene.hpp"
#include "camera.hpp"
#include "components/collisioncomponent.hpp"
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
  std::cout << "Game init \n";
}

void Scene::update() {
  // TODO Some wonkiness with collision sprite position and walking animation staying after changing
  // character
  controlSystem.update(transformSystem, *currentLevel.tcs.at(selectedPlayer), selectedPlayerIndex);

  if (selectedPlayer != selectedPlayerIndex) {
    selectedPlayer = selectedPlayerIndex;
    centerCamera(*currentLevel.tcs.at(selectedPlayer));
  };

  for (int i = 0; i < currentLevel.gcs.size(); i++) {
    gravitySystem.update(currentLevel.ccs.at(i)->getCollisionDown(), *currentLevel.gcs.at(i),
                         *currentLevel.tcs.at(i));
  }
  for (int i = 0; i < currentLevel.ccs.size(); i++) {
    collisionSystem.update(*currentLevel.ccs.at(i), *currentLevel.tcs.at(i), currentLevel);
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
