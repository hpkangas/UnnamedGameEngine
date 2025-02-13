#ifndef TRANSFORMSYSTEM_HPP
#define TRANSFORMSYSTEM_HPP
#include "../entities/entity.hpp"
class TransformSystem {
  float playerSpeed = 0.4f;
  float playerSpeedMax = 8.0f;

public:
  void update(Entity &selectedPlayer);
  void moveRight(Entity &selectedPlayer);
  void moveLeft(Entity &selectedPlayer);
  void moveUp(Entity &selectedPlayer);
  void moveDown(Entity &selectedPlayer);
  void stopX(Entity &selectedPlayer);
  void stopYU(Entity &selectedPlayer);
  void stopYD(Entity &selectedPlayer);
};
#endif