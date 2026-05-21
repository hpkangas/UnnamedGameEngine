#ifndef AISYSTEM_HPP
#define AISYSTEM_HPP
#include "../components/aicomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/healthcomponent.hpp"

#include "../scene/sceneinfo.hpp"

class AISystem {
public:
  void update(SceneInfo &currentLevel) const;
  void determineAIState(AIComponent &aiComponent, AttackComponent const &attackComponent,
                        HealthComponent const &healthComponent) const;
};

#endif
