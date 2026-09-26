#ifndef ATTACKSYSTEM_HPP
#define ATTACKSYSTEM_HPP
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../scene/sceneinfo.hpp"

struct AttackSystem {
  void update();
  void updateAttackComponents(AttackComponent &attackComponent,
                              PositionComponent const &positionComponent) const;
  void handleAttacks(AttackComponent const &attackComponent, entt::entity attackerEntity);
  void attack(AttackComponent const &attackComponent, HealthComponent &healthComponent,
              CollisionComponent const &collisionComponent) const;
  SceneInfo *currentLevel = nullptr;
};

#endif
