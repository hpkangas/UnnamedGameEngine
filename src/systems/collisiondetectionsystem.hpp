#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/transformcomponent.hpp"
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
                           TransformComponent const &playerTransformComponent) const;

  void checkProjectileCollision(CollisionComponent &cc, entt::entity entity) const;
  void checkProjectileCollisionWithTile(ProjectileComponent &projectile,
                           CollisionComponent &projectileCollision) const;
};

#endif
