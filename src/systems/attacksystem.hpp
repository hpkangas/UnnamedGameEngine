#ifndef ATTACKSYSTEM_HPP
#define ATTACKSYSTEM_HPP
#include "../components/attackcomponent.hpp"
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
