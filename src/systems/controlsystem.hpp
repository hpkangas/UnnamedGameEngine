#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/controlcomponent.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {}
  ~ControlSystem() {}
  void update(TransformSystem &transformSystem, TransformComponent &transformComponent,
              int &selectedPlayerIndex, ControlComponent &controlcomponent);
  void handleInput(TransformSystem &transformSystem, TransformComponent &transformComponent,
                   ControlComponent &controlComponent, int &selectedPlayerIndex);
  void changePlayerCharacter(int &selectedPlayerIndex);

  bool inputR = false;
  bool inputL = false;
  bool inputU = false;
  bool inputD = false;
  bool inputChangeCharacter = false;
};

#endif
    