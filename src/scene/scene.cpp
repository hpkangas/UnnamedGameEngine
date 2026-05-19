#include "scene.hpp"
#include "../camera.hpp"
#include "../components/transformcomponent.hpp"
#include "../system.hpp"
#include "../systems/transformsystem.hpp"
#include "../utilities/constants.hpp"
#include "scenecreator.hpp"
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
    Scene::isRunning = false;
    return false;
  } else {
    currentLevel = SceneCreator::createLevel(file);
    animationSystem.currentLevel = &currentLevel;
    std::cout << "Scene init \n";

    collisionDetectionSystem.currentLevel = &currentLevel;
    collisionSystem.currentLevel = &currentLevel;
    collisionSystem.transformSystem = &transformSystem;
    controlSystem.currentLevel = &currentLevel;
    controlSystem.transformSystem = &transformSystem;
    renderSystem.currentLevel = &currentLevel;
    attackSystem.currentLevel = &currentLevel;
    healthSystem.currentLevel = &currentLevel;

    Scene::isRunning = true;
    return true;
  }
}

void Scene::update() {
  if (selectedPlayer != selectedPlayerIndex) {
    selectedPlayer = selectedPlayerIndex;
    centerCamera(currentLevel.transformComponents.at(selectedPlayer));
  }

  for (int id : currentLevel.controlIds) {
    controlSystem.update(id, selectedPlayerIndex);
  }

  AISystem.update(currentLevel);

  for (int id : currentLevel.gravityIds) {
    gravitySystem.update(currentLevel.collisionComponents.at(id),
                         currentLevel.gravityComponents.at(id),
                         currentLevel.transformComponents.at(id));
  }
  collisionDetectionSystem.update();

  collisionSystem.update();

  for (int id : currentLevel.transformIds) {
    transformSystem.update(currentLevel.transformComponents.at(id));
  }

  attackSystem.update();
  healthSystem.update();

  animationSystem.update();

  renderSystem.update();

  updateCamera(currentLevel.transformComponents.at(selectedPlayer));
}

void Scene::render() const { renderSystem.render(); }

void Scene::updateCamera(const TransformComponent &transformComponent) const {
  const int xpos = transformComponent.position.x;
  const int ypos = transformComponent.position.y;
  const float xvel = transformComponent.velocity.x;
  const float yvel = transformComponent.velocity.y;

  if ((xpos < Camera::focusArea.left && xvel < 0) ||
      (xpos + Constants::PLAYER_WIDTH > Camera::focusArea.right && xvel > 0)) {
    Camera::offset.x += xvel;
  }

  if ((ypos < Camera::focusArea.top && yvel < 0) ||
      (ypos + Constants::PLAYER_HEIGHT > Camera::focusArea.bottom && yvel > 0)) {
    Camera::offset.y += yvel;
  }
  Camera::update();
}

void Scene::centerCamera(const TransformComponent &transformComponent) const {
  Camera::updateOffsetRelativeToScreen(transformComponent.position.x,
                                       transformComponent.position.y);
}
