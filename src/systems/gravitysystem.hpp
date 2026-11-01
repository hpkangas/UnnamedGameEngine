#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

class GravitySystem {
public:
  SceneInfo *currentLevel = nullptr;

  void gravity(VelocityComponent &velocityComponent) const;
  void update() const;

private:
  void updateGravity(GravityComponent &gc, VelocityComponent &vc, entt::entity entity) const;
};
#endif
