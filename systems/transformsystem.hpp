#ifndef TRANSFORMSYSTEM_HPP
#define TRANSFORMSYSTEM_HPP
#include "../entities/player.hpp"
class TransformSystem {
  float m_playerSpeed = 0.4f;
  float m_playerSpeedMax = 8.0f;

public:
  void update(Player &selectedPlayer);
  void moveRight(Player &selectedPlayer);
  void moveLeft(Player &selectedPlayer);
  void moveUp(Player &selectedPlayer);
  void moveDown(Player &selectedPlayer);
  void stopX(Player &selectedPlayer);
  void stopYU(Player &selectedPlayer);
  void stopYD(Player &selectedPlayer);
};
#endif