#include "controlsystem.hpp"
#include "../camera.hpp"
#include "../constants.hpp"
#include "../game.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "transformsystem.hpp"
#include <iostream>

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
        if (controlComponent.canClimb && !controlComponent.climbing) {
          controlComponent.climbing = true;
        }
        if (controlComponent.climbing) {
          inputU = true;
        } else {
          inputU = false;
        }
        break;
      case SDLK_DOWN:
        if (controlComponent.canClimb && !controlComponent.climbing) {
          controlComponent.climbing = true;
        }
        if (controlComponent.climbing) {
          inputD = true;
        } else {
          inputU = false;
        }
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
        transformComponent.setXvel(0);
        changePlayerCharacter(selectedPlayerIndex);
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

  handleInput(transformSystem, transformComponent, controlComponent.climbing);

  if (!controlComponent.canClimb) {
    controlComponent.climbing = false;
  }
}

void ControlSystem::handleInput(TransformSystem &transformSystem,
                                TransformComponent &transformComponent, bool grounded) {
  if (inputR && !inputL && !inputU && !inputD) {
    transformSystem.moveRight(transformComponent);
    return;
  }

  if (inputL && !inputR && !inputU && !inputD) {
    transformSystem.moveLeft(transformComponent);
    return;
  }

  if (inputU && !inputD) {
    transformSystem.moveUp(transformComponent);
    return;
  }

  if (inputD && !inputU) {
    transformSystem.moveDown(transformComponent);
    return;
  }

  transformSystem.stopX(transformComponent);
  if (grounded) {
    transformSystem.stopY(transformComponent);
  }
}

void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
