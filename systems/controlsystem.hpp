#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../camera.hpp"
#include "../entities/player.hpp"
#include "../level.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {};
  ~ControlSystem() {};
  void update(TransformSystem &transformSystem, Player &selectedPlayer, Camera &camera,
              LevelInfo &currentLevel);
  void handleInput(TransformSystem &transformSystem, Player &selectedPlayer);
  void changePlayerCharacter(TransformSystem &transformSystem, Player &selectedPlayer, Camera &camera, LevelInfo &currentLevel);
  void centerCameraToCharacter(const Player &player, Camera &camera);

  bool inputR = false;
  bool inputL = false;
};

#endif