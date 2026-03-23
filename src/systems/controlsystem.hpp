#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "transformsystem.hpp"

class ControlSystem {
public:
  void update(TransformSystem const &transformSystem, TransformComponent &transformComponent,
              int selectedPlayer, int &selectedPlayerIndex, ControlComponent &controlcomponent,
              CollisionComponent const &collisionComponent);

private:
  void determineControlState(ControlComponent &controlComponent,
                             CollisionComponent const &collisionComponent,
                             TransformComponent const &transformComponent) const;
  void takeInput(TransformComponent &transformComponent, int &selectedPlayerIndex);
  void handleInput(TransformSystem const &transformSystem, TransformComponent &transformComponent,
                   CollisionComponent const &collisionCOmponent, ControlComponent &controlComponent,
                   int &selectedPlayerIndex);
  void changePlayerCharacter(int &selectedPlayerIndex) const;

  bool inputR = false;
  bool inputL = false;
  bool inputU = false;
  bool inputD = false;
  bool inputChangeCharacter = false;
  bool inputResetCharacter = false;
  bool inputJump = false;
};

#endif
