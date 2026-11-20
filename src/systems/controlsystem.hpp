#ifndef CONTROLSYSTEM_HPP
#define CONTROLSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

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
  bool canJump = false;
  bool inputAttack = false;
  bool inputShoot = false;
  bool canAttack = false;
  bool canShoot = false;

public:
  SceneInfo *currentLevel = nullptr;
  void update(entt::entity &selectedPlayer);
};

#endif
