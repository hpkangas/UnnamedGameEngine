
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../scene/sceneinfo.hpp"
#include "transformsystem.hpp"

class CollisionSystem {
public:
  TransformSystem *transformSystem = nullptr;
  SceneInfo *currentLevel = nullptr;

  void update();

  void handleDownTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                               GravityComponent const &ctrlc) const;

  void handleUpTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                             GravityComponent const &ctrlc) const;
  void handleHorizontalBlockCollision(TransformComponent &tc, CollisionComponent const &cc) const;

  void handleProjectileCollision(ProjectileComponent const &pc, entt::entity entity) const;
};

#endif
