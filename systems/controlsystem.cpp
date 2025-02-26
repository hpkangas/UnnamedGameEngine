#include "controlsystem.hpp"
#include "../constants.hpp"
#include "../gamesystem.hpp"
#include "SDL_events.h"
#include "transformsystem.hpp"

void ControlSystem::update(TransformSystem &transformSystem, TransformComponent &transformComponent,
                           Camera &camera, LevelInfo &currentLevel) {

  SDL_Event event;

  // WaitForEvent pudottaa cpu% mutta tökkii
  if (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      // quit game
      case SDLK_q:
        GameSystem::isRunning = false;
        break;
      case SDLK_RIGHT:
        inputR = true;
        break;
      case SDLK_LEFT:
        inputL = true;
        break;
      // reset selectedPlayer pos for debugging
      case SDLK_r:
        transformComponent.setXvel(0);
        transformComponent.setYvel(0);
        transformComponent.setXpos(0);
        transformComponent.setYpos(0);
        // gravityComponent.setGravityState(INAIR);
        camera.m_position = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
        camera.m_offset = {0, 0};
        break;
      case SDLK_c:
        changePlayerCharacter(transformSystem, transformComponent, camera, currentLevel);
        break;
      default:
        break;
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_RIGHT:
        inputR = false;
        break;
      case SDLK_LEFT:
        inputL = false;
        break;
      default:
        break;
      }

    case SDL_WINDOWEVENT:
      switch (event.window.event) {
      case SDL_WINDOWEVENT_CLOSE:
        GameSystem::isRunning = false;
        break;
      default:
        break;
      }

    default:
      break;
    }
  }

  handleInput(transformSystem, transformComponent);
}

void ControlSystem::handleInput(TransformSystem &transformSystem,
                                TransformComponent &transformComponent) {
  if (inputR && !inputL) {
    transformSystem.moveRight(transformComponent);
    return;
  }

  if (inputL && !inputR) {
    transformSystem.moveLeft(transformComponent);
    return;
  }

  transformSystem.stopX(transformComponent);
}

void ControlSystem::changePlayerCharacter(TransformSystem &transformSystem,
                                          TransformComponent &transformComponent, Camera &camera,
                                          LevelInfo &currentLevel) {
  // if (selectedPlayer.getName() == "Erik") {
  //   transformSystem.stopX(selectedPlayer);
  //   //  selectedPlayer = *currentLevel.playercharacters.at(1);
  //   centerCameraToCharacter(selectedPlayer, camera);
  //   return;
  // }

  // if (selectedPlayer.getName() == "Olaf") {
  //   transformSystem.stopX(selectedPlayer);
  //   // selectedPlayer = currentLevel.playercharacters.at(2);
  //   centerCameraToCharacter(selectedPlayer, camera);
  //   return;
  // }

  // if (selectedPlayer.getName() == "Baleog") {
  //   transformSystem.stopX(selectedPlayer);
  //   //  selectedPlayer = currentLevel.playercharacters.at(0);
  //   centerCameraToCharacter(selectedPlayer, camera);
  // }
}

void ControlSystem::centerCameraToCharacter(TransformComponent &transformComponent,
                                            Camera &camera) {

  camera.updateOffsetRelativeToScreen(transformComponent.getXpos(), transformComponent.getYpos());
  camera.updatePositionRelativeToScreenAndOffset();
}
