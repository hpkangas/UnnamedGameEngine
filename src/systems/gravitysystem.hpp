#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../scene/sceneinfo.hpp"

class GravitySystem {
public:
  SceneInfo *currentLevel = nullptr;

  void gravity(TransformComponent &transformComponent) const;
  void update() const;

private:
  void updateOne(CollisionComponent const &collisionComponent,
                 GravityComponent &gravityComponent,
                 TransformComponent &transformComponent) const;
};
#endif
