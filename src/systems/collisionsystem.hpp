
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"
#include "transformsystem.hpp"

class CollisionSystem {
public:
  TransformSystem *transformSystem = nullptr;
  SceneInfo *currentLevel = nullptr;

  void update();

  void handleTileCollisions();

  void handleDownTileCollision(VelocityComponent &velocityComponent, PositionComponent &positionComponent, CollisionComponent const &cc,
                               GravityComponent const &ctrlc) const;

  void handleUpTileCollision(VelocityComponent &velocityComponent, PositionComponent &positionComponent, CollisionComponent const &cc,
                             GravityComponent const &ctrlc) const;
  void handleHorizontalBlockCollision(VelocityComponent &velocityComponent, PositionComponent &positionComponent, CollisionComponent const &cc) const;

  void handleProjectileCollisions();

};

#endif
