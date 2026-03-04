#include "transformsystem.hpp"
#include <iostream>
#include <iterator>
#include <ostream>

void TransformSystem::moveRight(TransformComponent &transformComponent) {
  if (transformComponent.getXvel()+playerSpeed>playerSpeedMax) {
    transformComponent.setXvel(playerSpeedMax);
  }else {
    transformComponent.setXvel(transformComponent.getXvel() + playerSpeed);
  }
}

void TransformSystem::moveLeft(TransformComponent &transformComponent) {
  if (transformComponent.getXvel() < -playerSpeedMax) {
    transformComponent.setXvel(-playerSpeedMax);
  } else {
    transformComponent.setXvel(transformComponent.getXvel() - playerSpeed);
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
  // transformComponent.jumpTics = std::size(transformComponent.jumpTable) - 1;
  transformComponent.setYvel(-12);
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
