#include "scene.hpp"
#include "../camera.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../system.hpp"
#include "../systems/transformsystem.hpp"
#include "../utilities/constants.hpp"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <iostream>

bool Scene::init() {
  const char *fileName = "level2.csv";
  SDL_RWops *file = SDL_RWFromFile(fileName, "rb");
  if (file == nullptr) {
    std::cout << "Error: " << SDL_GetError() << std::endl;
    return false;
  } else {
    currentLevel = SceneCreator::createLevel(file);
    std::cout << "Scene init \n";
    return true;
  }
}

void Scene::update() {
  if (selectedPlayer != selectedPlayerIndex) {
    selectedPlayer = selectedPlayerIndex;
    centerCamera(*currentLevel.transformsComponents.at(selectedPlayer));
  }

  for (int id : currentLevel.controlIds) {
    controlSystem.update(transformSystem, *currentLevel.transformsComponents.at(id), id,
                         selectedPlayerIndex, *currentLevel.controlComponents.at(id),
                         *currentLevel.collisionComponents.at(id));
  }

  AISystem.update(currentLevel);

  for (int id : currentLevel.gravityIds) {
    gravitySystem.update(*currentLevel.collisionComponents.at(id),
                         *currentLevel.gravityComponents.at(id),
                         *currentLevel.transformsComponents.at(id));
  }
  collisionDetectionSystem.update(currentLevel);

  collisionSystem.update(currentLevel, transformSystem);

  for (int id : currentLevel.transformIds) {
    transformSystem.update(*currentLevel.transformsComponents.at(id));
  }

  for (int id : currentLevel.animationIds) {
    animationSystem.update(
        *currentLevel.controlComponents.at(id), *currentLevel.collisionComponents.at(id),
        currentLevel.gravityComponents.at(id)->gravityState,
        *currentLevel.transformsComponents.at(id), *currentLevel.renderComponents.at(id),
        *currentLevel.animationComponents.at(id));
  }

  renderSystem.update(currentLevel);

  updateCamera(*currentLevel.transformsComponents.at(selectedPlayer));
}

void Scene::render() { renderSystem.render(currentLevel); }

void Scene::updateCamera(const TransformComponent &transformComponent) const {
  const int xpos = transformComponent.position.x;
  const int ypos = transformComponent.position.y;
  const float xvel = transformComponent.velocity.x;
  const float yvel = transformComponent.velocity.y;

  if ((xpos < Camera::focusArea.x && xvel < 0) ||
      (xpos + PLAYER_WIDTH > Camera::focusArea.w && xvel > 0)) {
    Camera::offset.x += xvel;
  }

  if ((ypos < Camera::focusArea.y && yvel < 0) ||
      (ypos + PLAYER_HEIGHT > Camera::focusArea.h && yvel > 0)) {
    Camera::offset.y += yvel;
  }
  Camera::update();
}

void Scene::centerCamera(const TransformComponent &transformComponent) const {
  Camera::updateOffsetRelativeToScreen(transformComponent.position.x,
                                       transformComponent.position.y);
}
