#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../scene/sceneinfo.hpp"
#include "transformsystem.hpp"

class ControlSystem {

  void determineControlState(entt::entity selectedPlayer);
  void takeInput();
  void resetInput();
  void handleInput(entt::entity selectedPlayer);
  void checkClimbing(VelocityComponent &velocityComponent, ControlComponent &controlComponent,
                     CollisionComponent const &collisionComponent) const;

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
  //TODO non class function or static function for transformSystem
  TransformSystem *transformSystem = nullptr;
  void update(entt::entity &selectedPlayer);
};

#endif
