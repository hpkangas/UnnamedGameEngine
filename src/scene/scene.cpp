#include "scene.hpp"
#include <SDL2/SDL_error.h>
#include "../camera.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../utilities/constants.hpp"
#include "../system.hpp"
#include "../systems/transformsystem.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>

bool Scene::init() {
  const char *fileName = "level2";
    SDL_RWops *file = SDL_RWFromFile(fileName, "rb");
    if (file == nullptr) {
      std::cout<<"Error: "<<SDL_GetError()<<std::endl;
      return false;
    }
    else {
      currentLevel = SceneCreator::createLevel(file);
      std::cout << "Scene init \n";
      return true;
    }
  }


void Scene::update() {
  if (selectedPlayer != selectedPlayerIndex) {
    selectedPlayer = selectedPlayerIndex;
    centerCamera(*currentLevel.tcs.at(selectedPlayer));
  }

  for (int id : currentLevel.controlIds) {
    controlSystem.update(transformSystem, *currentLevel.tcs.at(id), id, selectedPlayerIndex,
                         *currentLevel.ctrlComponents.at(id),
                         *currentLevel.ccs.at(id));
  }
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

  for (int id : currentLevel.transformIds) {
    transformSystem.update(*currentLevel.tcs.at(id));
  }

  for (int id : currentLevel.animationIds) {
    animationSystem.update(*currentLevel.ctrlComponents.at(id), *currentLevel.ccs.at(id),
                           currentLevel.gcs.at(id)->gravityState, *currentLevel.tcs.at(id),
                           *currentLevel.rcs.at(id), *currentLevel.acs.at(id));
  }
  for (int id : currentLevel.renderIds) {
    renderSystem.update(*currentLevel.tcs.at(id), *currentLevel.rcs.at(id));
  }

  updateCamera(*currentLevel.tcs.at(selectedPlayer));
}

void Scene::render() { renderSystem.renderComponents(currentLevel.rcs, currentLevel.renderIds); }

void Scene::updateCamera(const TransformComponent &transformComponent) const {
  const int xpos = transformComponent.position.x;
  const int ypos = transformComponent.position.y;
  const float xvel = transformComponent.velocity.x;
  const float yvel = transformComponent.velocity.y;

  if ((xpos < Camera::position.x && xvel < 0) ||
      (xpos + PLAYER_WIDTH > Camera::position.w &&
       xvel > 0)) {
    Camera::offset.x += xvel;
  }

  if ((ypos < Camera::position.y && yvel < 0) ||
      (ypos + PLAYER_HEIGHT > Camera::position.h &&
       yvel > 0)) {
    Camera::offset.y += yvel;
  }
  Camera::updatePositionRelativeToScreenAndOffset();
}

void Scene::centerCamera(const TransformComponent &transformComponent) const {
  Camera::updateOffsetRelativeToScreen(transformComponent.position.x, transformComponent.position.y);
}
