
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

class CollisionSystem {
public:
  SceneInfo *currentLevel = nullptr;

  void update();

  void handleTileCollisions();

  void handleDownTileCollision(VelocityComponent &vc, PositionComponent &pc, CollisionComponent const &cc) const;
  void handleUpTileCollision(VelocityComponent &vc, PositionComponent &pc, CollisionComponent const &cc) const;
  void handleHorizontalBlockCollision(VelocityComponent &vc, PositionComponent &pc, CollisionComponent const &cc) const;

  void handleProjectileCollisions();

};

#endif
