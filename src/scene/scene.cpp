#include "scene.hpp"
#include "../camera.hpp"
#include "../system.hpp"
#include "../systems/transformsystem.hpp"
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
    selectedPlayer = currentLevel.playerEntity;

    animationSystem.currentLevel = &currentLevel;
    std::cout << "Scene init \n";

    collisionDetectionSystem.currentLevel = &currentLevel;
    collisionSystem.currentLevel = &currentLevel;
    collisionSystem.transformSystem = &transformSystem;
    controlSystem.currentLevel = &currentLevel;
    controlSystem.transformSystem = &transformSystem;
    gravitySystem.currentLevel = &currentLevel;
    transformSystem.currentLevel = &currentLevel;
    renderSystem.currentLevel = &currentLevel;
    attackSystem.currentLevel = &currentLevel;
    shootSystem.currentLevel = &currentLevel;
    healthSystem.currentLevel = &currentLevel;

    Scene::isRunning = true;
    return true;
  }
}

void Scene::update() {
  controlSystem.update(selectedPlayer);

  AISystem.update(currentLevel);

  gravitySystem.update();

  collisionDetectionSystem.update();

  collisionSystem.update();

  transformSystem.updateAll();

  attackSystem.update();
  shootSystem.update();
  healthSystem.update();

  animationSystem.update();

  renderSystem.update();

  updateCamera(currentLevel.registry.get<PositionComponent>(selectedPlayer),
               currentLevel.registry.get<VelocityComponent>(selectedPlayer));
}

void Scene::render() const { renderSystem.render(); }

void Scene::updateCamera(const PositionComponent &positionComponent,
                         const VelocityComponent &velocityComponent) const {
  const int xpos = positionComponent.position.x;
  const int ypos = positionComponent.position.y;
  const float xvel = velocityComponent.velocity.x;
  const float yvel = velocityComponent.velocity.y;

  if ((xpos < Camera::focusArea.left && xvel < 0) || (xpos > Camera::focusArea.right && xvel > 0)) {
    Camera::offset.x += xvel;
  }

  if ((ypos < Camera::focusArea.top && yvel < 0) || (ypos > Camera::focusArea.bottom && yvel > 0)) {
    Camera::offset.y += yvel;
  }
  Camera::update();
}

void Scene::centerCamera(const PositionComponent &transformComponent) const {
  Camera::updateOffsetRelativeToScreen(transformComponent.position.x,
                                       transformComponent.position.y);
}
