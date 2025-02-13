#include "controlsystem.hpp"
#include "../components/gravitycomponent.hpp"
#include "../constants.hpp"
#include "../gamesystem.hpp"
#include "SDL_events.h"
#include "transformsystem.hpp"

void ControlSystem::update(TransformSystem &transformSystem, Entity &selectedPlayer, Camera &camera,
                           LevelInfo &currentLevel) {

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
        selectedPlayer.transformComponent.setXvel(0);
        selectedPlayer.transformComponent.setYvel(0);
        selectedPlayer.transformComponent.setXpos(0);
        selectedPlayer.transformComponent.setYpos(0);
        selectedPlayer.gravityComponent.setGravityState(INAIR);
        camera.m_position = {SCREENWIDTH * 1 / 4, SCREENWIDTH * 3 / 4, 0, 0};
        camera.m_offset = {0, 0, 0, 0};
        break;
      case SDLK_c:
        changePlayerCharacter(transformSystem, selectedPlayer, camera, currentLevel);
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
    default:
      break;
    }
  }

  handleInput(transformSystem, selectedPlayer);
}

void ControlSystem::handleInput(TransformSystem &transformSystem, Entity &selectedPlayer) {
  if (inputR && !inputL) {
    transformSystem.moveRight(selectedPlayer);
    return;
  }

  if (inputL && !inputR) {
    transformSystem.moveLeft(selectedPlayer);
    return;
  }

  transformSystem.stopX(selectedPlayer);
}

void ControlSystem::changePlayerCharacter(TransformSystem &transformSystem, Entity &selectedPlayer,
                                          Camera &camera, LevelInfo &currentLevel) {
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

void ControlSystem::centerCameraToCharacter(const Entity &player, Camera &camera) {

  camera.updateOffsetRelativeToScreen(player.transformComponent.getXpos(),
                                      player.transformComponent.getYpos());
  camera.updatePositionRelativeToScreenAndOffset();
}
