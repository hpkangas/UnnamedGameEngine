#ifndef TRANSFORMSYSTEM_HPP
#define TRANSFORMSYSTEM_HPP
#include "../components/transformcomponent.hpp"

class TransformSystem {
  float playerSpeed = 0.4f;
  float playerSpeedMax = 8.0f;
  float playerClimbingSpeedMax = 4.0f;

public:
  void update(TransformComponent &transformComponent);
  void moveRight(TransformComponent &transformComponent);
  void moveLeft(TransformComponent &transformComponent);
  void moveUp(TransformComponent &transformComponent);
  void moveDown(TransformComponent &transformComponent);
  void stopX(TransformComponent &transformComponent);
  void stopX(TransformComponent &transformComponent, int xposDif);
  void stopY(TransformComponent &transformComponent);
  void stopY(TransformComponent &transformComponent, int yposDif);
  void jump(TransformComponent &transformComponent);
  void resetJump(TransformComponent &transformComponent);
};
#endif
