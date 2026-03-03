#include "controlsystem.hpp"
#include "../camera.hpp"
#include "../constants.hpp"
#include "../game.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "transformsystem.hpp"
#include <cstdio>
#include <iostream>

void ControlSystem::update(TransformSystem &transformSystem, TransformComponent &transformComponent,
                           int selectedPlayer, int &selectedPlayerIndex,
                           ControlComponent &controlComponent, GravityComponent &gravityComponent,
                           CollisionComponent &collisionComponent) {

  this->transformSystem = &transformSystem;
  inputR = false;
  inputL = false;
  inputD = false;
  inputU = false;
  inputJump = false;
  inputResetCharacter = false;
  determineControlState(controlComponent, gravityComponent.getGravityState(), collisionComponent,
                        transformComponent);

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

  // tuplahyppy tulee siitä kun collision detection
  switch (collisionComponent.collisionTileDown) {
  case TILE::NONE:
    printf("asd");
    controlComponent.setControlState(CONTROL_STATE::AIRBORNE);

    break;

  case TILE::SOLID:
    if (controlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(transformComponent, collisionComponent.yposDif);
      controlComponent.setControlState(CONTROL_STATE::NORMAL);
    }
    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:

    break;

  case TILE::LADDERSTART:
    if (controlComponent.getControlState() == CONTROL_STATE::AIRBORNE) {
      transformSystem->stopY(transformComponent, collisionComponent.yposDif);
      controlComponent.setControlState(CONTROL_STATE::NORMAL);
    }

    break;

  default:
    break;
  }

  if (collisionComponent.collisionTileLeft == TILE::SOLID ||
      collisionComponent.collisionTileRight == TILE::SOLID) {
    transformSystem->stopX(transformComponent, collisionComponent.xposDif);
  }
}

void ControlSystem::takeInput(TransformComponent &transformComponent, int &selectedPlayerIndex) {
  SDL_Event event;

  if (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      Game::isRunning = false;
    }
  }
  const Uint8 *keyboardStateArray = SDL_GetKeyboardState(NULL);
  if (keyboardStateArray[SDL_SCANCODE_Q]) {
    Game::isRunning = false;
  }
  if (keyboardStateArray[SDL_SCANCODE_RIGHT]) {
    inputR = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_LEFT]) {
    inputL = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_D]) {
    inputJump = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_UP]) {
    inputU = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_DOWN]) {
    inputD = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_R]) {
    inputResetCharacter = true;
  }
}

void ControlSystem::handleInput(TransformSystem &transformSystem,
                                TransformComponent &transformComponent,
                                CollisionComponent &collisionComponent,
                                ControlComponent &controlComponent, int &selectedPlayerIndex) {
  // for debugging
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

  bool noInput = true;

  switch (controlComponent.getControlState()) {
  case CONTROL_STATE::NORMAL:
    std::cout << "normal" << std ::endl;

    if (inputR && !inputL) {
      if (transformComponent.getXvel() < 0) {
        transformSystem.stopX(transformComponent);
      }
      transformSystem.moveRight(transformComponent);
      noInput = false;
    }

    if (inputL && !inputR) {
      if (transformComponent.getXvel() > 0) {
        transformSystem.stopX(transformComponent);
      }
      transformSystem.moveLeft(transformComponent);
      noInput = false;
    }

    if (inputU) {
      noInput = false;
      if (controlComponent.getCanClimb()) {

        transformComponent.setXpos(collisionComponent.collidedUpTilePos.x);

        transformSystem.stopX(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        return;
      }
    }

    if (inputD) {
      noInput = false;
      if (controlComponent.getCanClimb()) {
        // not sure I like this direct setting of xpos
        transformComponent.setXpos(collisionComponent.collidedDownTilePos.x);

        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        return;
      }
    }

    if (inputJump) {
      noInput = true;
      std::cout << "handling jump input" << std::endl;
      transformSystem.jump(transformComponent);
      // transformComponent.setYvel(-20);
      // controlComponent.setControlState(CONTROL_STATE::AIRBORNE);
    }

    if (noInput) {
      transformSystem.stopX(transformComponent);
    }

    break;

  case CONTROL_STATE::AIRBORNE:
    std::cout << "airborne" << std ::endl;

    if (inputR && !inputL) {
      noInput = false;
      if (transformComponent.getXvel() < 0) {
        transformSystem.stopX(transformComponent);
      }
      transformSystem.moveRight(transformComponent);
    }

    if (inputL && !inputR) {
      noInput = false;
      if (transformComponent.getXvel() > 0) {
        transformSystem.stopX(transformComponent);
      }
      transformSystem.moveLeft(transformComponent);
    }

    if (inputU) {

      if (controlComponent.getCanClimb()) {
        std::cout << "can climb from air \n";
        // not sure I like this direct setting of xpos
        transformComponent.setXpos(collisionComponent.collidedUpTilePos.x);

        // SHould systems call other systems?
        // maybe but I think they should be put on the systems "list of things to do" rather than do
        // it immediately (which is not possible at the moment)
        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
      }
    }

    if (inputD) {

      if (controlComponent.getCanClimb()) {
        transformComponent.setXpos(collisionComponent.collidedDownTilePos.x);

        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
      }
    }

    if (noInput) {
      transformSystem.stopX(transformComponent);
    }

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
}

void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
