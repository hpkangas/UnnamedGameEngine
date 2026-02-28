#include "transformsystem.hpp"
#include <iostream>
#include <ostream>

void TransformSystem::moveRight(TransformComponent &transformComponent) {
  transformComponent.setXvel(transformComponent.getXvel() + playerSpeed);
  if (transformComponent.getXvel() > playerSpeedMax) {
    transformComponent.setXvel(playerSpeedMax);
  }
}

void TransformSystem::moveLeft(TransformComponent &transformComponent) {
  transformComponent.setXvel(transformComponent.getXvel() - playerSpeed);
  if (transformComponent.getXvel() < -playerSpeedMax) {
    transformComponent.setXvel(-playerSpeedMax);
  }
}

void TransformSystem::moveUp(TransformComponent &transformComponent) {
  transformComponent.setYvel(transformComponent.getYvel() - 8);
  if (transformComponent.getYvel() < -playerClimbingSpeedMax) {
    transformComponent.setYvel(-playerClimbingSpeedMax);
  }
}

void TransformSystem::moveDown(TransformComponent &transformComponent) {
  transformComponent.setYvel(transformComponent.getYvel() + 8);
  if (transformComponent.getYvel() > playerClimbingSpeedMax) {
    transformComponent.setYvel(playerClimbingSpeedMax);
  }
}

void TransformSystem::jump(TransformComponent &transformComponent) {
  transformComponent.jumpTics = 2;
}

void TransformSystem::resetJump(TransformComponent &transformComponent) {}

void TransformSystem::stopX(TransformComponent &transformComponent) {
  stopX(transformComponent, 0);
}

void TransformSystem::stopX(TransformComponent &transformComponent, int xposDif) {

  if (transformComponent.getXvel() != 0) {
    if (xposDif != 0) {
      transformComponent.setXpos(transformComponent.getXpos() - xposDif);
    }
    transformComponent.setXvel(0);
  }
}

void TransformSystem::stopY(TransformComponent &transformComponent) {
  stopY(transformComponent, 0);
}

void TransformSystem::stopY(TransformComponent &transformComponent, int yposDif) {
  if (yposDif != 0) {
    transformComponent.setYpos(transformComponent.getYpos() - yposDif);
  }
  transformComponent.setYvel(0);
}

void TransformSystem::update(TransformComponent &transformComponent) {

  if (transformComponent.jumpTics > -1) {
    std::cout << "jump tics: " << transformComponent.jumpTics << std::endl;
    transformComponent.setYvel(transformComponent.getYvel() -
                               transformComponent.jumpTable[transformComponent.jumpTics]);
    transformComponent.jumpTics--;
  }

  const float xvel = transformComponent.getXvel();
  const float yvel = transformComponent.getYvel();
  const int xpos = transformComponent.getXpos();
  const int ypos = transformComponent.getYpos();

  if (!(xvel == 0)) {
    transformComponent.setXpos(xpos + xvel);
  }

  if (!(yvel == 0)) {
    transformComponent.setYpos(ypos + yvel);
  }
}
