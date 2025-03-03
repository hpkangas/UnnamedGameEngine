#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {}
  ~ControlSystem() {}
  void update(TransformSystem &transformSystem, TransformComponent &transformComponent,
              int &selectedPlayerIndex);
  void handleInput(TransformSystem &transformSystem, TransformComponent &transformComponent);
  void changePlayerCharacter(int &selectedPlayerIndex);

  bool inputR = false;
  bool inputL = false;
};

#endif
