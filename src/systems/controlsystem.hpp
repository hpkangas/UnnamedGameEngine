#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../components/shootcomponent.hpp"
#include "../scene/sceneinfo.hpp"
#include "transformsystem.hpp"

class ControlSystem {

  void determineControlState(ControlComponent &controlComponent,
                             CollisionComponent const &collisionComponent,
                             TransformComponent const &transformComponent,
                             AttackComponent &attackComponent,
                             HealthComponent const &healthComponent,
                             ShootComponent &shootComponent);
  void takeInput();
  void resetInput();
  void handleInput(TransformComponent &transformComponent,
                   CollisionComponent const &collisionComponent,
                   ControlComponent &controlComponent);

  bool inputR = false;
  bool inputL = false;
  bool inputU = false;
  bool inputD = false;
  bool inputChangeCharacter = false;
  bool inputResetCharacter = false;
  bool inputJump = false;
  bool canJump = true;
  bool inputAttack = false;
  bool inputShoot = false;
  bool canAttack = false;
  bool canShoot = false;

public:
  SceneInfo *currentLevel = nullptr;
  TransformSystem *transformSystem = nullptr;
  void update(entt::entity &selectedPlayer);
};

#endif
