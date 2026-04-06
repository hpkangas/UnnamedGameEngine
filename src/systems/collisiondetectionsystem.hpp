
#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../scene/scenecreator.hpp"

class CollisionDetectionSystem {
public:
  void update(SceneInfo &currentLevel) const;
  void checkCollision(int i, SceneInfo &currentLevel) const;

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
};

#endif
