#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../scene/sceneinfo.hpp"

class CollisionDetectionSystem {
public:
  SceneInfo *currentLevel = nullptr;
  void update();
  void checkTileCollision(entt::entity entity) const;

  void checkPlayerLeftCollision(CollisionComponent &playerC, CollisionComponent const &block,
                                Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkPlayerRightCollision(CollisionComponent &playerC, CollisionComponent const &block,
                                 Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkPlayerUpCollision(CollisionComponent &playerC, CollisionComponent const &block,
                              Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkPlayerDownCollision(CollisionComponent &playerC, CollisionComponent const &block,
                                Utils::Velocity velocity, TILE const &collisionTile) const;

  void resetCollisions(CollisionComponent &collisionComponent) const;

  void updateCollisionRect(CollisionComponent &playerCollisionComponent,
                           PositionComponent const &playerPositionComponent) const;

  void checkProjectileCollision(ProjectileComponent &projectile,
                           CollisionComponent const &projectileCollision) const;
};

#endif
