#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

class CollisionDetectionSystem {
public:
  SceneInfo *currentLevel = nullptr;
  void update();
  void checkTileCollision(CollisionComponent &object, PositionComponent const &position,
                          VelocityComponent const &velocity) const;

  void checkEntityLeftTileCollision(CollisionComponent &objectCollision, CollisionComponent const &tileCollision,
                                    Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkEntityRightTileCollision(CollisionComponent &objectCollision, CollisionComponent const &tileCollision,
                                     Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkEntityUpTileCollision(CollisionComponent &objectCollision, CollisionComponent const &tileCollision,
                                  Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkEntityDownTileCollision(CollisionComponent &objectCollision, CollisionComponent const &tileCollision,
                                    Utils::Velocity velocity, TILE const &collisionTile) const;

  void resetCollisions(CollisionComponent &collisionComponent) const;

  void updateCollisionRect(CollisionComponent &objectCollisionComponent,
                           PositionComponent const &objectPositionComponent) const;

  void checkProjectileCollision(ProjectileComponent &projectile,
                                CollisionComponent const &projectileCollision) const;
};

#endif
