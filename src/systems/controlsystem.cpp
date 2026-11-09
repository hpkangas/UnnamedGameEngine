#include "controlsystem.hpp"
#include "../components/controlcomponent.hpp"
#include "../scene/scene.hpp"
#include "../system.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "transformsystem.hpp"
#include <cstdio>

void ControlSystem::update(entt::entity &selectedPlayer) {

  determineControlState(selectedPlayer);
  takeInput();
  handleInput(selectedPlayer);
}

void ControlSystem::determineControlState(entt::entity selectedPlayer) {

  auto &reg = currentLevel->registry;
  ControlComponent &controlComponent = reg.get<ControlComponent>(selectedPlayer);
  PositionComponent &transformComponent = reg.get<PositionComponent>(selectedPlayer);
  VelocityComponent &velocityComponent = reg.get<VelocityComponent>(selectedPlayer);
  CollisionComponent const &collisionComponent = reg.get<CollisionComponent>(selectedPlayer);
  AttackComponent &attackComponent = reg.get<AttackComponent>(selectedPlayer);
  HealthComponent const &healthComponent = reg.get<HealthComponent>(selectedPlayer);
  ShootComponent &shootComponent = reg.get<ShootComponent>(selectedPlayer);
  if (healthComponent.healthState == HEALTH_STATE::DEAD) {
    controlComponent.controlState = CONTROL_STATE::DEAD;
    return;
  }

  const auto &downTileTypes = collisionComponent.collidedDownTileTypes;
  const auto &upTileTypes = collisionComponent.collidedUpTileTypes;

  if (downTileTypes.empty()) {
    if (controlComponent.controlState != CONTROL_STATE::AIRBORNE) {
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
    }
  } else if (downTileTypes.hasTileType(TILE::SOLID)) {
    if (controlComponent.controlState == CONTROL_STATE::AIRBORNE &&
        velocityComponent.velocity.y == 0) {
      controlComponent.controlState = CONTROL_STATE::NORMAL;
    }

    if (controlComponent.controlState == CONTROL_STATE::ATTACKING) {
      if (attackComponent.currentAttackFrames == attackComponent.attackFrames) {
        attackComponent.currentAttackFrames = 0;
        if (!inputAttack) {
          attackComponent.attacking = false;
          canAttack = true;
          controlComponent.controlState = CONTROL_STATE::NORMAL;
        }
      } else {
        attackComponent.attacking = true;
        attackComponent.currentAttackFrames++;
      }
    }

    if (controlComponent.controlState == CONTROL_STATE::SHOOTING) {
      if (shootComponent.currentShootingFrame == shootComponent.shootingFrames) {
        shootComponent.currentShootingFrame = 0;
        if (!inputShoot) {
          shootComponent.shooting = false;
          canShoot = true;
          controlComponent.controlState = CONTROL_STATE::NORMAL;
        }
      } else {
        shootComponent.shooting = true;
        shootComponent.currentShootingFrame++;
      }
    }
  }

  checkClimbing(velocityComponent, controlComponent, collisionComponent);
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
  if (!keyboardStateArray[SDL_SCANCODE_RIGHT]) {
    inputR = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_LEFT]) {
    inputL = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_LEFT]) {
    inputL = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_A]) {
    inputAttack = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_A]) {
    canAttack = true;
    inputAttack = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_S]) {
    inputShoot = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_S]) {
    canShoot = true;
    inputShoot = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_D]) {
    inputJump = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_D]) {
    canJump = true;
    inputJump = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_UP]) {
    inputU = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_UP]) {
    inputU = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_DOWN]) {
    inputD = true;
  }
  if (!keyboardStateArray[SDL_SCANCODE_DOWN]) {
    inputD = false;
  }

  if (keyboardStateArray[SDL_SCANCODE_R]) {
    inputResetCharacter = true;
  }
}

void ControlSystem::handleInput(entt::entity selectedPlayer) {
  auto &reg = currentLevel->registry;
  ControlComponent &controlComponent = reg.get<ControlComponent>(selectedPlayer);
  PositionComponent &positionComponent = reg.get<PositionComponent>(selectedPlayer);
  VelocityComponent &velocityComponent = reg.get<VelocityComponent>(selectedPlayer);
  CollisionComponent const &collisionComponent = reg.get<CollisionComponent>(selectedPlayer);

  // for debugging
  if (inputResetCharacter) {
    velocityComponent.velocity.x = 0;
    velocityComponent.velocity.y = 0;
    positionComponent.position.x = 0;
    positionComponent.position.y = 0;
    controlComponent.controlState = CONTROL_STATE::AIRBORNE;
    // Camera::focusArea = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
    // Camera::offset = {0, -200};
    return;
  }

  bool noInput = true;

  switch (controlComponent.controlState) {
  case CONTROL_STATE::NORMAL:

    if (inputR && !inputL) {
      if (velocityComponent.velocity.x < 0) {
        stopX(velocityComponent, positionComponent);
      }
      moveRight(velocityComponent, positionComponent);
      noInput = false;
    }

    if (inputL && !inputR) {
      if (velocityComponent.velocity.x > 0) {
        stopX(velocityComponent, positionComponent);
      }
      moveLeft(velocityComponent, positionComponent);
      noInput = false;
    }

    if (inputU) {
      noInput = false;
      if (controlComponent.canClimbUp) {

        positionComponent.position.x = collisionComponent.collidedUpTilePos.x;

        stopX(velocityComponent, positionComponent);
        stopY(velocityComponent, positionComponent);

        controlComponent.controlState = CONTROL_STATE::CLIMBING;
        return;
      }
    }

    if (inputD) {
      noInput = false;
      if (controlComponent.canClimbDown) {
        positionComponent.position.x = collisionComponent.collidedDownTilePos.x;

        stopX(velocityComponent, positionComponent);
        stopY(velocityComponent, positionComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
        return;
      }
    }

    if (inputJump && canJump) {
      noInput = false;
      jump(velocityComponent);
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
      canJump = false;
    }

    if (inputAttack && canAttack) {
      noInput = false;
      controlComponent.controlState = CONTROL_STATE::ATTACKING;
      canAttack = false;
    }

    if (inputShoot && canShoot) {
      noInput = false;
      controlComponent.controlState = CONTROL_STATE::SHOOTING;
      canShoot = false;
    }

    if (noInput) {
      stopX(velocityComponent, positionComponent);
    }

    break;

  case CONTROL_STATE::AIRBORNE:

    if (inputR && !inputL) {
      noInput = false;
      if (velocityComponent.velocity.x < 0) {
        stopX(velocityComponent, positionComponent);
      }
      moveRight(velocityComponent, positionComponent);
    }

    if (inputL && !inputR) {
      noInput = false;
      if (velocityComponent.velocity.x > 0) {
        stopX(velocityComponent, positionComponent);
      }
      moveLeft(velocityComponent, positionComponent);
    }

    if (inputU) {
      if (controlComponent.canClimbUp) {
        positionComponent.position.x = collisionComponent.collidedUpTilePos.x;
        stopX(velocityComponent, positionComponent);
        stopY(velocityComponent, positionComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
      }
    }

    if (inputD) {
      if (controlComponent.canClimbDown) {
        positionComponent.position.x = collisionComponent.collidedDownTilePos.x;
        stopX(velocityComponent, positionComponent);
        stopY(velocityComponent, positionComponent);
        controlComponent.controlState = CONTROL_STATE::CLIMBING;
      }
    }

    if (noInput) {
      stopX(velocityComponent, positionComponent);
    }

    break;

  case CONTROL_STATE::FALLING:

    break;

  case CONTROL_STATE::CLIMBING:

    if (!collisionComponent.collidedDownTileTypes.hasTileType(TILE::LADDERSTART) &&
        !collisionComponent.collidedUpTileTypes.hasTileType(TILE::LADDERSTART)) {
      if (inputR && !inputL && !inputU && !inputD) {
        controlComponent.controlState = CONTROL_STATE::AIRBORNE;
        stopY(velocityComponent, positionComponent);
        return;
      }

      if (inputL && !inputR && !inputU && !inputD) {
        controlComponent.controlState = CONTROL_STATE::AIRBORNE;
        stopY(velocityComponent, positionComponent);
        return;
      }
    }

    if (inputU) {
      if (controlComponent.canClimbUp) {
        moveUp(velocityComponent);
      }
      return;
    }

    if (inputD) {
      if (!collisionComponent.collidedDownTileTypes.hasTileType(TILE::LADDER) &&
          !collisionComponent.collidedDownTileTypes.hasTileType(TILE::LADDERSTART)) {
        controlComponent.controlState = CONTROL_STATE::NORMAL;
      } else if (controlComponent.canClimbDown) {
        moveDown(velocityComponent);
      }
      return;
    }

    stopY(velocityComponent, positionComponent);
    break;

  case CONTROL_STATE::ATTACKING:

    break;

  default:
    break;
  }

  controlComponent.canClimbDown = false;
  controlComponent.canClimbUp = false;
}

void ControlSystem::checkClimbing(VelocityComponent &velocityComponent,
                                  ControlComponent &controlComponent,
                                  CollisionComponent const &collisionComponent) const {

  const auto &downTileTypes = collisionComponent.collidedDownTileTypes;
  const auto &upTileTypes = collisionComponent.collidedUpTileTypes;

  if (downTileTypes.hasTileType(TILE::LADDER)) {
    controlComponent.canClimbDown = true;
    if (velocityComponent.velocity.x != 0 &&
        controlComponent.controlState == CONTROL_STATE::CLIMBING) {
      controlComponent.controlState = CONTROL_STATE::AIRBORNE;
    }
  }

  if (downTileTypes.hasTileType(TILE::LADDERSTART)) {
    if (controlComponent.controlState == CONTROL_STATE::AIRBORNE &&
        velocityComponent.velocity.y == 0) {
      controlComponent.controlState = CONTROL_STATE::NORMAL;
    }
    controlComponent.canClimbDown = true;
  }

  if (upTileTypes.empty()) {
    if (downTileTypes.hasTileType(TILE::LADDER) ||
        downTileTypes.hasTileType(TILE::LADDERSTART) &&
            controlComponent.controlState == CONTROL_STATE::CLIMBING) {
      controlComponent.canClimbUp = true;
    }
  }
  if (upTileTypes.hasTileType(TILE::LADDER)) {
    controlComponent.canClimbUp = true;
    if (downTileTypes.hasTileType(TILE::LADDER) &&
        controlComponent.controlState == CONTROL_STATE::NORMAL) {
      controlComponent.canClimbDown = false;
    }
  }

  if (upTileTypes.hasTileType(TILE::LADDERSTART)) {
    controlComponent.canClimbUp = true;
  }
}

void ControlSystem::resetInput() {
  inputR = false;
  inputL = false;
  inputD = false;
  inputU = false;
  inputJump = false;
  inputAttack = false;
  inputShoot = false;
  inputResetCharacter = false;
}
