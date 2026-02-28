#include "controlsystem.hpp"
#include "../camera.hpp"
#include "../constants.hpp"
#include "../game.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "transformsystem.hpp"
#include <cstdio>
#include <iostream>

void ControlSystem::update(TransformSystem &transformSystem, TransformComponent &transformComponent,
                           int selectedPlayer, int &selectedPlayerIndex,
                           ControlComponent &controlComponent, GravityComponent &gravityComponent,
                           CollisionComponent &collisionComponent) {

  this->transformSystem = &transformSystem;

  // determineControlState(controlComponent, gravityComponent.getGravityState(), collisionComponent,
  //                       transformComponent);

  if (selectedPlayer == selectedPlayerIndex) {

    takeInput(transformComponent, selectedPlayerIndex);
    handleInput(transformSystem, transformComponent, collisionComponent, controlComponent,
                selectedPlayerIndex);
  }
}

void ControlSystem::determineControlState(ControlComponent &controlComponent,
                                          GRAVITY_STATE gravityState,
                                          CollisionComponent &collisionComponent,
                                          TransformComponent &transformComponent) {
  //   controlComponent.setControlState(CONTROL_STATE::AIRBORNE);
  // } else {
  //   controlComponent.setControlState(CONTROL_STATE::NORMAL);
  // }
  // if (gravityState == GRAVITY_STATE::AIRBORNE) {
  //   if (transformComponent.getYvel() == 0) {
  //     controlComponent.setControlState(CONTROL_STATE::NORMAL);
  //   }
  // }
}

void ControlSystem::takeInput(TransformComponent &transformComponent, int &selectedPlayerIndex) {
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
        inputResetCharacter = true;
        break;
      case SDLK_c:
        inputChangeCharacter = true;
        break;
      case SDLK_d:
        if (inputJump) {
          inputJump = false;
          std::cout << "jump false" << std::endl;
        } else {
          inputJump = true;
          std::cout << "jump true" << std::endl;
        }
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
      case SDLK_r:
        inputResetCharacter = false;
        break;
      case SDLK_c:
        inputChangeCharacter = false;
        break;
      case SDLK_d:
        inputJump = false;
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
}

void ControlSystem::handleInput(TransformSystem &transformSystem,
                                TransformComponent &transformComponent,
                                CollisionComponent &collisionComponent,
                                ControlComponent &controlComponent, int &selectedPlayerIndex) {

  if (inputResetCharacter) {
    transformComponent.setXvel(0);
    transformComponent.setYvel(0);
    transformComponent.setXpos(0);
    transformComponent.setYpos(-200);
    controlComponent.setControlState(CONTROL_STATE::AIRBORNE);
    Camera::position = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
    Camera::offset = {0, -200};
    return;
  }

  if (inputChangeCharacter) {
    changePlayerCharacter(selectedPlayerIndex);
    transformComponent.setXvel(0);
    transformComponent.setYvel(0);
    inputChangeCharacter = false;
    return;
  }

  switch (controlComponent.getControlState()) {
  case CONTROL_STATE::NORMAL:
    std::cout << "normal" << std::endl;

    if (inputR && !inputL) {
      transformSystem.moveRight(transformComponent);
      return;
    }

    if (inputL && !inputR) {
      transformSystem.moveLeft(transformComponent);
      return;
    }

    if (inputU) {
      if (collisionComponent.collisionTileUp == TILE::LADDER ||
          collisionComponent.collisionTileUp == TILE::LADDERSTART) {

        transformComponent.setXpos(collisionComponent.collidedUpTilePos.x);

        transformSystem.stopX(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        // not sure I like this direct setting of xpos
        return;
      }
    }

    if (inputD) {
      if (collisionComponent.collisionTileDown == TILE::LADDER ||
          collisionComponent.collisionTileDown == TILE::LADDERSTART) {
        transformComponent.setXpos(collisionComponent.collidedDownTilePos.x);

        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        return;
      }
    }

    if (inputJump) {
      std::cout << "handling jump input" << std::endl;
      transformSystem.jump(transformComponent);
      return;
    }

    transformSystem.stopX(transformComponent);
    break;

  case CONTROL_STATE::AIRBORNE:
    std::cout << "airborne" << std::endl;

    if (inputR && !inputL) {
      transformSystem.moveRight(transformComponent);
      return;
    }

    if (inputL && !inputR) {
      transformSystem.moveLeft(transformComponent);
      return;
    }
    // No input

    if (inputU) {

      if (controlComponent.getCanClimb()) {

        // not sure I like this direct setting of xpos
        transformComponent.setXpos(collisionComponent.collidedUpTilePos.x);

        // SHould systems call other systems?
        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        return;
      }
    }

    if (inputD) {

      if (controlComponent.getCanClimb()) {
        transformComponent.setXpos(collisionComponent.collidedDownTilePos.x);

        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        return;
      }
    }

    transformSystem.stopX(transformComponent);

    break;

  case CONTROL_STATE::FALLING:

    break;

  case CONTROL_STATE::CLIMBING:

    if (inputR && !inputL && !inputU && !inputD) {
      controlComponent.setControlState(CONTROL_STATE::AIRBORNE);
      transformSystem.moveRight(transformComponent);
      return;
    }

    if (inputL && !inputR && !inputU && !inputD) {
      controlComponent.setControlState(CONTROL_STATE::AIRBORNE);
      transformSystem.moveLeft(transformComponent);
      return;
    }

    if (inputU) {
      if (collisionComponent.collisionTileUp != TILE::LADDER &&
          collisionComponent.collisionTileUp != TILE::LADDERSTART &&
          collisionComponent.collisionTileDown != TILE::LADDER &&
          collisionComponent.collisionTileDown != TILE::LADDERSTART) {

      } else {
        transformSystem.stopX(transformComponent);
        transformSystem.moveUp(transformComponent);
      }

      return;
    }

    if (inputD) {

      if (collisionComponent.collisionTileDown != TILE::LADDER &&
          collisionComponent.collisionTileDown != TILE::LADDERSTART) {
        controlComponent.setControlState(CONTROL_STATE::NORMAL);
        transformSystem.stopY(transformComponent);

      } else {
        transformSystem.stopX(transformComponent);
        transformSystem.moveDown(transformComponent);
      }
      return;
    }

    transformSystem.stopY(transformComponent);
    break;

  default:
    break;
  }
  inputJump = false;
}

void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
