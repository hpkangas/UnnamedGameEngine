#include "controlsystem.hpp"
#include "../components/gravitycomponent.hpp"
#include "../constants.hpp"
#include "../gamesystem.hpp"
#include "SDL_events.h"

void ControlSystem::update(Player &selectedPlayer, Camera &camera, LevelInfo &currentLevel) {

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
        changePlayerCharacter(selectedPlayer, camera, currentLevel);
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

  handleInput(selectedPlayer);
}

void ControlSystem::handleInput(Player &selectedPlayer) {
  if (inputR && !inputL) {
    selectedPlayer.moveRight();
    return;
  }

  if (inputL && !inputR) {
    selectedPlayer.moveLeft();
    return;
  }

  selectedPlayer.stopX();
}

void ControlSystem::changePlayerCharacter(Player &selectedPlayer, Camera &camera,
                                          LevelInfo &currentLevel) {
  if (selectedPlayer.getName() == "Erik") {
    selectedPlayer.stopX();
    //  selectedPlayer = *currentLevel.playercharacters.at(1);
    centerCameraToCharacter(selectedPlayer, camera);
    return;
  }

  if (selectedPlayer.getName() == "Olaf") {
    selectedPlayer.stopX();
    // selectedPlayer = currentLevel.playercharacters.at(2);
    centerCameraToCharacter(selectedPlayer, camera);
    return;
  }

  if (selectedPlayer.getName() == "Baleog") {
    selectedPlayer.stopX();
    //  selectedPlayer = currentLevel.playercharacters.at(0);
    centerCameraToCharacter(selectedPlayer, camera);
  }
}

void ControlSystem::centerCameraToCharacter(const Player &player, Camera &camera) {

  camera.updateOffsetRelativeToScreen(player.transformComponent.getXpos(),
                                      player.transformComponent.getYpos());
  camera.updatePositionRelativeToScreenAndOffset();
}
