#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

using namespace Utils;
class CollisionDetectionSystem {
public:
  SceneInfo *currentLevel = nullptr;
  void update();

  void updateCollisionComponents();

  void checkEntityCollisions();

  void checkTileCollisions();

  void checkTileCollision(CollisionComponent &object, PositionComponent const &position,
                          VelocityComponent const &velocity, entt::entity const entity) const;

  void checkEntityLeftTileCollision(CollisionComponent &objectCollision,
                                    CollisionComponent const &tileCollision, Velocity objVel,
                                    Velocity const &tileVel, TILE const &collisionTile) const;
  void checkEntityRightTileCollision(CollisionComponent &objectCollision,
                                     CollisionComponent const &tileCollision, Velocity objVel,
                                     Velocity const &tileVel, TILE const &collisionTile) const;
  void checkEntityUpTileCollision(CollisionComponent &objectCollision,
                                  CollisionComponent const &tileCollision, Velocity objVel,
                                  Velocity const &tileVel, TILE const &collisionTile) const;
  void checkEntityDownTileCollision(CollisionComponent &objectCollision,
                                    CollisionComponent const &tileCollision, Velocity objVel,
                                    Velocity const &tileVel, TILE const &collisionTile) const;

  void checkEntityDownRightTileCollision(CollisionComponent &objectCollision,
                                         CollisionComponent const &tileCollision, Velocity objVel,
                                         Velocity const &tileVel, TILE const &collisionTile) const;

  void checkEntityDownLeftTileCollision(CollisionComponent &objectCollision,
                                        CollisionComponent const &tileCollision, Velocity objVel,
                                        Velocity const &tileVel, TILE const &collisionTile) const;

  void resetCollisions(CollisionComponent &collisionComponent) const;

  void updateCollisionRect(CollisionComponent &objectCollisionComponent,
                           PositionComponent const &objectPositionComponent) const;

  void checkProjectileCollision(ProjectileComponent &projectile,
                                CollisionComponent const &projectileCollision) const;
};

#endif
