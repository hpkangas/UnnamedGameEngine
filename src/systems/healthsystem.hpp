#ifndef HEALTHSYSTEM_HPP
#define HEALTHSYSTEM_HPP
#include "../scene/sceneinfo.hpp"
struct HealthSystem {

  void update();
  SceneInfo *currentLevel = nullptr;
};
#endif
