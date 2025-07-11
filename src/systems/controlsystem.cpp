#include "controlsystem.hpp"
#include "../camera.hpp"
#include "../constants.hpp"
#include "../game.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "transformsystem.hpp"

void ControlSystem::update(TransformSystem &transformSystem, TransformComponent &transformComponent,
                           int &selectedPlayerIndex, ControlComponent &controlComponent) {

  SDL_Event event;
  // WaitForEvent pudottaa cpu% mutta tökkii
  if (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      // quit game
      case SDLK_q:
        Game::isRunning = false;
        break;
      case SDLK_RIGHT:
        inputR = true;
        break;
      case SDLK_LEFT:
        inputL = true;
        break;
      case SDLK_UP:
        inputU = true;
        break;
      case SDLK_DOWN:
        inputD = true;
        break;
      // reset selectedPlayer pos for debugging
      case SDLK_r:
        transformComponent.setXvel(0);
        transformComponent.setYvel(0);
        transformComponent.setXpos(0);
        transformComponent.setYpos(0);
        Camera::position = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
        Camera::offset = {0, 0};
        break;
      case SDLK_c:
        inputChangeCharacter = true;

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
      case SDLK_UP:
        inputU = false;
        break;
      case SDLK_DOWN:
        inputD = false;
        break;
      default:
        break;
      }
      break;

    case SDL_WINDOWEVENT:
      switch (event.window.event) {
      case SDL_WINDOWEVENT_CLOSE:
        Game::isRunning = false;
        break;
      default:
        break;
      }
      break;

    default:
      break;
    }
  }

  handleInput(transformSystem, transformComponent, controlComponent, selectedPlayerIndex);
}

void ControlSystem::handleInput(TransformSystem &transformSystem,
                                TransformComponent &transformComponent,
                                ControlComponent &controlComponent, int &selectedPlayerIndex) {
  if (inputChangeCharacter) {
    changePlayerCharacter(selectedPlayerIndex);
    transformComponent.setXvel(0);
    transformComponent.setYvel(0);
    inputChangeCharacter = false;
    return;
  }
  if (!controlComponent.canClimb) {
    controlComponent.climbing = false;
  }

  if (controlComponent.canClimb && !controlComponent.climbing) {

    if (inputU) {
      controlComponent.climbing = true;
      // not sure I like this direct setting of xpos
      transformComponent.setXpos(controlComponent.collidedTilePos.x);

      transformSystem.stopX(transformComponent);
      transformSystem.stopY(transformComponent);
      return;
    }

    if (inputD) {
      controlComponent.climbing = true;
      transformSystem.stopX(transformComponent);
      transformSystem.stopY(transformComponent);
      return;
    }
  }

  // Should this be determined elsewhere?

  if (controlComponent.climbing == true) {

    if (inputR && !inputL && !inputU && !inputD) {
      controlComponent.climbing = false;
      transformSystem.moveRight(transformComponent);
      return;
    }

    if (inputL && !inputR && !inputU && !inputD) {
      controlComponent.climbing = false;
      transformSystem.moveLeft(transformComponent);
      return;
    }

    if (inputU) {
      transformSystem.stopX(transformComponent);
      transformSystem.moveUp(transformComponent);
      return;
    }

    if (inputD) {
      transformSystem.stopX(transformComponent);
      transformSystem.moveDown(transformComponent);
      return;
    }
  }

  if (inputR && !inputL) {
    transformSystem.moveRight(transformComponent);
    return;
  }

  if (inputL && !inputR) {
    transformSystem.moveLeft(transformComponent);
    return;
  }

  // No input
  transformSystem.stopX(transformComponent);
  if (controlComponent.climbing) {
    transformSystem.stopY(transformComponent);
  }
}

void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
