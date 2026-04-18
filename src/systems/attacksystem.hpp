#ifndef ATTACKSYSTEM_HPP
#define ATTACKSYSTEM_HPP
#include "../scene/sceneinfo.hpp"

struct AttackSystem {
  void update();
  void updateAttackComponents(AttackComponent &attackComponent,
                              TransformComponent const &transformComponent) const;
  void checkAttackCollisions(AttackComponent const &attackComponent, int attackerId,
                             bool facingRight);
  SceneInfo *currentLevel = nullptr;
};

#endif
