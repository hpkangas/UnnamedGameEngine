#include "../camera.hpp"
#include "../entities/player.hpp"
#include "../level.hpp"

#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP

class ControlSystem {
public:
  ControlSystem() {};
  ~ControlSystem() {};
  void update(Player &selectedPlayer, Camera &camera, LevelInfo &currentLevel);
  void handleInput(Player &selectedPlayer);
  void changePlayerCharacter(Player &selectedPlayer, Camera &camera, LevelInfo &currentLevel);
  void centerCameraToCharacter(const Player &player, Camera &camera);

  bool inputR = false;
  bool inputL = false;
};

#endif