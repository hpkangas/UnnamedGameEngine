#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../scene/sceneinfo.hpp"
#include "transformsystem.hpp"

class ControlSystem {

  void determineControlState(ControlComponent &controlComponent,
                             CollisionComponent const &collisionComponent,
                             TransformComponent const &transformComponent,
                             AttackComponent &attackComponent);
  void takeInput();
  void handleInput(TransformComponent &transformComponent,
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
  bool canJump = true;
  bool inputAttack = false;
  bool canAttack = true;

public:
  SceneInfo *currentLevel = nullptr;
  TransformSystem *transformSystem = nullptr;
  void update(int selectedPlayer, int &selectedPlayerIndex);
};

#endif
