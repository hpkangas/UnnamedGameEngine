#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../camera.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {}
  ~ControlSystem() {}
  void update(TransformSystem &transformSystem, TransformComponent &transformComponent,
              Camera &camera, int &selectedPlayerIndex);
  void handleInput(TransformSystem &transformSystem, TransformComponent &transformComponent);
  void changePlayerCharacter(int &selectedPlayerIndex);

  bool inputR = false;
  bool inputL = false;
};

#endif
