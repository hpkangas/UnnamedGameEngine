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

  std::cout << "-------" << "\n";
  std::cout << "controlled plaeyr " << selectedPlayerIndex << "\n";
  std::cout << "selected player" << selectedPlayer << "\n";
  this->transformSystem = &transformSystem;

  determineControlState(controlComponent, gravityComponent.getGravityState(), collisionComponent,
                        transformComponent);

  if (selectedPlayer == selectedPlayerIndex) {

    takeInput(transformComponent, selectedPlayerIndex);
    handleInput(transformSystem, transformComponent, collisionComponent, controlComponent,
                selectedPlayerIndex);
  }

  std::cout << "---------" << "\n";
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

  std::cout << "CONTROL SYSTEM" << "\n";

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
    std::cout << "control normal \n";
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
        std::cout << "input up \n";

        transformComponent.setXpos(collisionComponent.collidedUpTilePos.x);

        transformSystem.stopX(transformComponent);
        // transformSystem.stopY(transformComponent);
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

    transformSystem.stopX(transformComponent);
    break;

  case CONTROL_STATE::AIRBORNE:
    std::cout << "control  airborjne \n";

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
      std::cout << "input up airborjne\n";

      if (controlComponent.getCanClimb()) {
        std::cout << "climb up \n";

        // not sure I like this direct setting of xpos
        transformComponent.setXpos(collisionComponent.collidedUpTilePos.x);

        transformSystem.stopX(transformComponent);
        transformSystem.stopY(transformComponent);
        controlComponent.setControlState(CONTROL_STATE::CLIMBING);
        return;
      }
    }

    if (inputD) {
      std::cout << "input down airborjne\n";

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
    std::cout << "control falling \n";

    break;

  case CONTROL_STATE::CLIMBING:
    std::cout << "control climbing \n";

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
      std::cout << "input up climbing\n";
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
      std::cout << "input down climbing\n";

      if (collisionComponent.collisionTileDown != TILE::LADDER &&
          collisionComponent.collisionTileDown != TILE::LADDERSTART) {
        controlComponent.setControlState(CONTROL_STATE::NORMAL);
        transformSystem.stopY(transformComponent, collisionComponent.yposDif);

      } else {
        transformSystem.stopX(transformComponent);
        // bugi: menee pikselin tai pari liian alas törmäävään blockiin nähen
        transformSystem.moveDown(transformComponent);
      }
      return;
    }

    transformSystem.stopY(transformComponent);
    break;
  }
}

void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
