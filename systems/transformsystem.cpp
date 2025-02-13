#include "transformsystem.hpp"
#include "../components/transformcomponent.hpp"

void TransformSystem::moveRight(Entity &selectedPlayer) {
  selectedPlayer.transformComponent.setXvel(selectedPlayer.transformComponent.getXvel() +
                                            playerSpeed);
  if (selectedPlayer.transformComponent.getXvel() > playerSpeedMax)
    selectedPlayer.transformComponent.setXvel(playerSpeedMax);
}

void TransformSystem::moveLeft(Entity &selectedPlayer) {
  selectedPlayer.transformComponent.setXvel(selectedPlayer.transformComponent.getXvel() -
                                            playerSpeed);
  if (selectedPlayer.transformComponent.getXvel() < -playerSpeedMax)
    selectedPlayer.transformComponent.setXvel(-playerSpeedMax);
}

void TransformSystem::moveUp(Entity &selectedPlayer) {
  selectedPlayer.transformComponent.setYvel(-playerSpeed);
}

// debugging only mostly
void TransformSystem::moveDown(Entity &selectedPlayer) {
  selectedPlayer.transformComponent.setYvel(playerSpeed);
}

// debugging only mostly
void TransformSystem::stopX(Entity &selectedPlayer) {
  if (selectedPlayer.transformComponent.getXvel() != 0) {
    selectedPlayer.transformComponent.setXvel(0);
  }
}

// debugging only mostly
void TransformSystem::stopYU(Entity &selectedPlayer) {
  if (selectedPlayer.transformComponent.velocity.y < 0) {
    selectedPlayer.transformComponent.velocity.y = 0;
  }
}

// debugging only mostly
void TransformSystem::stopYD(Entity &selectedPlayer) {
  if (selectedPlayer.transformComponent.velocity.y > 0) {
    selectedPlayer.transformComponent.velocity.y = 0;
  }
}

void TransformSystem::update(Entity &selectedPlayer) {
  const float xvel = selectedPlayer.transformComponent.getXvel();
  const float yvel = selectedPlayer.transformComponent.getYvel();
  const int xpos = selectedPlayer.transformComponent.getXpos();
  const int ypos = selectedPlayer.transformComponent.getYpos();

  if (!(xvel == 0)) {
    selectedPlayer.transformComponent.setXpos(xpos + xvel);
  }

  if (!(yvel == 0)) {
    selectedPlayer.transformComponent.setYpos(ypos + yvel);
  }

  if (xvel < 0) {
    selectedPlayer.renderComponent.flip = SDL_FLIP_HORIZONTAL;
  }

  if (xvel > 0) {
    selectedPlayer.renderComponent.flip = SDL_FLIP_NONE;
  }
}