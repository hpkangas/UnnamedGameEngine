#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  ControlSystem() {}
  ~ControlSystem() {}

  TransformSystem *transformSystem;

  void update(TransformSystem &transformSystem, TransformComponent &transformComponent,
              int selectedPlayer, int &selectedPlayerIndex, ControlComponent &controlcomponent,
              GravityComponent &gravityComponent, CollisionComponent &collisionComponent);

private:
  void determineControlState(ControlComponent &controlComponent, GRAVITY_STATE gravityState,
                             CollisionComponent &collisionComponent,
                             TransformComponent &transformComponent);
  void takeInput(TransformComponent &transformComponent, int &selectedPlayerIndex);
  void handleInput(TransformSystem &transformSystem, TransformComponent &transformComponent,
                   CollisionComponent &collisionCOmponent, ControlComponent &controlComponent,
                   int &selectedPlayerIndex);
  void changePlayerCharacter(int &selectedPlayerIndex);

  bool inputR = false;
  bool inputL = false;
  bool inputU = false;
  bool inputD = false;
  bool inputChangeCharacter = false;
  bool inputResetCharacter = false;
};

#endif
