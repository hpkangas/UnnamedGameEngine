#ifndef ATTACKSYSTEM_HPP
#define ATTACKSYSTEM_HPP
#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../scene/sceneinfo.hpp"

struct AttackSystem {
  void update();
  void updateAttackComponents(AttackComponent &attackComponent,
                              TransformComponent const &transformComponent) const;
  void checkAttackCollisions(AttackComponent const &attackComponent, entt::entity attackerEntity,
                             bool facingRight);
  SceneInfo *currentLevel = nullptr;
};

#endif
