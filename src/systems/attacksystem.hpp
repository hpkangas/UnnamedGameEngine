#ifndef ATTACKSYSTEM_HPP
#define ATTACKSYSTEM_HPP
#include "../scene/sceneinfo.hpp"

struct AttackSystem {
  void update();
  void updateAttackComponents(AttackComponent &attackComponent,
                              TransformComponent &transformComponent);
  void checkAttackCollisions(AttackComponent const &attackComponent, int attackerId,
                             Utils::Velocity vel);
  SceneInfo *currentLevel = nullptr;
};

#endif
