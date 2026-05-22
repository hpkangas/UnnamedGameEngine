#ifndef ATTACKSYSTEM_HPP
#define ATTACKSYSTEM_HPP
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../scene/sceneinfo.hpp"

struct AttackSystem {
  void update();
  void updateAttackComponents(AttackComponent &attackComponent,
                              TransformComponent const &transformComponent) const;
  void handleAttacks(AttackComponent const &attackComponent, entt::entity attackerEntity,
                     bool facingRight);
  void attack(AttackComponent const &attackComponent, bool facing, HealthComponent &healthComponent,
              CollisionComponent const &collisionComponent) const;
  SceneInfo *currentLevel = nullptr;
};

#endif
