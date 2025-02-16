#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../camera.hpp"
#include "../level.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {};
  ~ControlSystem() {};
  void update(TransformSystem &transformSystem, TransformComponent &transformComponent,
              Camera &camera, LevelInfo &currentLevel);
  void handleInput(TransformSystem &transformSystem, TransformComponent &transformComponent);
  void changePlayerCharacter(TransformSystem &transformSystem,
                             TransformComponent &transformComponent, Camera &camera,
                             LevelInfo &currentLevel);
  void centerCameraToCharacter(TransformComponent &transformComponent, Camera &camera);

  bool inputR = false;
  bool inputL = false;
};

#endif