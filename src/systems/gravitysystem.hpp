#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../scene/sceneinfo.hpp"

class GravitySystem {
public:
  // Wired by Scene::init so update() can iterate the gravity pool.
  SceneInfo *currentLevel = nullptr;

  void gravity(TransformComponent &transformComponent) const;

  // Drives the per-entity step from the gravity pool directly. The
  // previous `update(collision, gravity, transform)` per-entity entry
  // point is gone — Scene no longer needs to know about per-component
  // iteration.
  void update() const;

private:
  void updateOne(CollisionComponent const &collisionComponent,
                 GravityComponent &gravityComponent,
                 TransformComponent &transformComponent) const;
};
#endif
