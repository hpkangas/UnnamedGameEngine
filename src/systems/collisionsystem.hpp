
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

class CollisionSystem {
public:
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
