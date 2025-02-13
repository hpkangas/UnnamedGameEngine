#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../camera.hpp"
#include "../level.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {};
  ~ControlSystem() {};
  void update(TransformSystem &transformSystem, Entity &selectedPlayer, Camera &camera,
              LevelInfo &currentLevel);
  void handleInput(TransformSystem &transformSystem, Entity &selectedPlayer);
  void changePlayerCharacter(TransformSystem &transformSystem, Entity &selectedPlayer, Camera &camera, LevelInfo &currentLevel);
  void centerCameraToCharacter(const Entity &player, Camera &camera);

  bool inputR = false;
  bool inputL = false;
};

#endif