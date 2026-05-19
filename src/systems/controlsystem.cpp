#include "controlsystem.hpp"
#include "../camera.hpp"
#include "../scene/scene.hpp"
#include "../system.hpp"
#include "../utilities/constants.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "transformsystem.hpp"
#include <cstdio>
#include <iostream>

using namespace Constants;
void ControlSystem::update(int selectedPlayer, int &selectedPlayerIndex) {

  TransformComponent &transformComponent = currentLevel->transformComponents.at(selectedPlayer);
  ControlComponent &controlComponent = currentLevel->controlComponents.at(selectedPlayer);
  CollisionComponent const &collisionComponent =
      currentLevel->collisionComponents.at(selectedPlayer);
  AttackComponent &attackComponent = currentLevel->attackComponents.at(selectedPlayer);
  HealthComponent const &healthComponent = currentLevel->healthComponents.at(selectedPlayer);

  resetInput();

  determineControlState(controlComponent, collisionComponent, transformComponent, attackComponent,
                        healthComponent);

  if (selectedPlayer == selectedPlayerIndex) {
    takeInput();
    handleInput(transformComponent, collisionComponent, controlComponent, selectedPlayerIndex);
  }
}

void ControlSystem::determineControlState(ControlComponent &controlComponent,
                                          CollisionComponent const &collisionComponent,
                                          TransformComponent const &transformComponent,
                                          AttackComponent &attackComponent,
                                          HealthComponent const &healthComponent) {

  if (healthComponent.healthState == HEALTH_STATE::DEAD) {
    controlComponent.controlState = CONTROL_STATE::DEAD;
    return;
  }

  switch (collisionComponent.collisionTileDown) {
  case TILE::NONE:
    if (controlComponent.controlState != CONTROL_STATE::AIRBORNE) {
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
    }
    break;

  case TILE::SOLID:
    if (controlComponent.controlState == CONTROL_STATE::AIRBORNE &&
        transformComponent.velocity.y == 0) {
      controlComponent.controlState = CONTROL_STATE::NORMAL;
    }

    if (controlComponent.controlState == CONTROL_STATE::ATTACKING &&
        attackComponent.currentAttackFrames == attackComponent.attackFrames - 1) {
      controlComponent.controlState = CONTROL_STATE::NORMAL;
      canAttack = true;
      attackComponent.currentAttackFrames = 0;
      attackComponent.attacking = false;

    } else if (controlComponent.controlState == CONTROL_STATE::ATTACKING) {
      attackComponent.attacking = true;
      attackComponent.currentAttackFrames++;
      break;
    }

    break;

  case TILE::SEMISOLID:
    break;

  case TILE::LADDER:

    break;

  case TILE::LADDERSTART:
    if (controlComponent.controlState == CONTROL_STATE::AIRBORNE) {
      if (transformComponent.velocity.y == 0) {
        controlComponent.controlState = CONTROL_STATE::NORMAL;
      }
      // transformSystem->stopY(transformComponent, collisionComponent.yposDif);
      // controlComponent.controlState = CONTROL_STATE::NORMAL);
    }

    break;

  default:
    break;
  }

  // if (collisionComponent.collisionTileLeft == TILE::SOLID ||
  //     collisionComponent.collisionTileRight == TILE::SOLID) {
  //   transformSystem->stopX(transformComponent, collisionComponent.xposDif);
  // }
}

void ControlSystem::takeInput() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      Scene::isRunning = false;
      System::isRunning = false;
      return;
    }
  }

  const Uint8 *keyboardStateArray = SDL_GetKeyboardState(nullptr);
  if (keyboardStateArray[SDL_SCANCODE_F1]) {
    Scene::isRunning = false;
    return;
  }
  if (keyboardStateArray[SDL_SCANCODE_F2]) {
    Scene::isRunning = false;
    System::isRunning = false;
    return;
  }
  if (keyboardStateArray[SDL_SCANCODE_RIGHT]) {
    inputR = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_LEFT]) {
    inputL = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_A]) {
    inputAttack = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_A]) {
    canAttack = true;
  }
  if (keyboardStateArray[SDL_SCANCODE_D]) {
    inputJump = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_D]) {
    canJump = true;
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

void ControlSystem::handleInput(TransformComponent &transformComponent,
                                CollisionComponent const &collisionComponent,
                                ControlComponent &controlComponent, int &selectedPlayerIndex) {
  // for debugging
  if (inputResetCharacter) {
    transformComponent.velocity.x = 0;
    transformComponent.velocity.y = 0;
    transformComponent.position.x = 0;
    transformComponent.position.y = 0;
    controlComponent.controlState = CONTROL_STATE::AIRBORNE;
    Camera::focusArea = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
    Camera::offset = {0, -200};
    return;
  }

  if (inputChangeCharacter) {
    changePlayerCharacter(selectedPlayerIndex);
    transformComponent.velocity.x = 0;
    transformComponent.velocity.y = 0;
    inputChangeCharacter = false;
    return;
  }

  bool noInput = true;

  switch (controlComponent.controlState) {
  case CONTROL_STATE::NORMAL:

    if (inputR && !inputL) {
      if (transformComponent.velocity.x < 0) {
        transformSystem->stopX(transformComponent);
      }
      transformSystem->moveRight(transformComponent);
      noInput = false;
    }

    if (inputL && !inputR) {
      if (transformComponent.velocity.x > 0) {
        transformSystem->stopX(transformComponent);
      }
      transformSystem->moveLeft(transformComponent);
      noInput = false;
    }

    if (inputU) {
      noInput = false;
      if (controlComponent.canClimb) {

        transformComponent.position.x = collisionComponent.collidedUpTilePos.x;

        transformSystem->stopX(transformComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
        return;
      }
    }

    if (inputD) {
      noInput = false;
      if (controlComponent.canClimb) {
        // not sure I like this direct setting of xpos
        transformComponent.position.x = collisionComponent.collidedDownTilePos.x;

        transformSystem->stopX(transformComponent);
        transformSystem->stopY(transformComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
        return;
      }
    }

    if (inputJump && canJump) {
      noInput = false;
      transformSystem->jump(transformComponent);
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
      canJump = false;
    }

    if (inputAttack && canAttack) {
      noInput = false;
      controlComponent.controlState = CONTROL_STATE::ATTACKING;
      canAttack = false;
    }

    if (noInput) {
      transformSystem->stopX(transformComponent);
    }

    break;

  case CONTROL_STATE::AIRBORNE:

    if (inputR && !inputL) {
      noInput = false;
      if (transformComponent.velocity.x < 0) {
        transformSystem->stopX(transformComponent);
      }
      transformSystem->moveRight(transformComponent);
    }

    if (inputL && !inputR) {
      noInput = false;
      if (transformComponent.velocity.x > 0) {
        transformSystem->stopX(transformComponent);
      }
      transformSystem->moveLeft(transformComponent);
    }

    if (inputU) {

      if (controlComponent.canClimb) {
        std::cout << "can climb from air \n";
        // not sure I like this direct setting of xpos
        transformComponent.position.x = collisionComponent.collidedUpTilePos.x;

        // SHould systems call other systems?
        // maybe but I think they should be put on the systems "list of things to do" rather than
        // do it immediately (which is not possible at the moment)
        transformSystem->stopX(transformComponent);
        transformSystem->stopY(transformComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
      }
    }

    if (inputD) {

      if (controlComponent.canClimb) {
        transformComponent.position.x = collisionComponent.collidedDownTilePos.x;

        transformSystem->stopX(transformComponent);
        transformSystem->stopY(transformComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
      }
    }

    if (noInput) {
      transformSystem->stopX(transformComponent);
    }

    break;

  case CONTROL_STATE::FALLING:

    break;

  case CONTROL_STATE::CLIMBING:

    if (inputR && !inputL && !inputU && !inputD) {
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
      transformSystem->moveRight(transformComponent);
      return;
    }

    if (inputL && !inputR && !inputU && !inputD) {
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
      transformSystem->moveLeft(transformComponent);
      return;
    }

    if (inputU) {
      if (collisionComponent.collisionTileUp != TILE::LADDER &&
          collisionComponent.collisionTileUp != TILE::LADDERSTART &&
          collisionComponent.collisionTileDown != TILE::LADDER &&
          collisionComponent.collisionTileDown != TILE::LADDERSTART) {
      } else {
        transformSystem->stopX(transformComponent);
        transformSystem->moveUp(transformComponent);
      }

      return;
    }

    if (inputD) {

      if (collisionComponent.collisionTileDown != TILE::LADDER &&
          collisionComponent.collisionTileDown != TILE::LADDERSTART) {
        controlComponent.controlState = CONTROL_STATE::NORMAL;
        transformSystem->stopY(transformComponent);

      } else {
        transformSystem->stopX(transformComponent);
        transformSystem->moveDown(transformComponent);
      }
      return;
    }

    transformSystem->stopY(transformComponent);
    break;

  case CONTROL_STATE::ATTACKING:

    break;

  default:
    break;
  }
}

void ControlSystem::resetInput() {
  inputR = false;
  inputL = false;
  inputD = false;
  inputU = false;
  inputJump = false;
  inputAttack = false;
  inputResetCharacter = false;
}

void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) const {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
