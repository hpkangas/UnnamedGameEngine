
#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../scene/scenecreator.hpp"

class CollisionDetectionSystem {
public:
  TILE currentTile = TILE::NONE;
  Utils::Position collidedDownObjectPos = {0, 0};
  Utils::Position collidedUpObjectPos = {0, 0};

  void update(int i, SceneInfo &currentLevel) const;
  void checkBlockCollision(int i, SceneInfo &currentLevel) const;

  void checkPlayerLeftCollision(CollisionComponent &playerC, CollisionComponent const &block,
                                Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkPlayerRightCollision(CollisionComponent &playerC, CollisionComponent const &block,
                                 Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkPlayerUpCollision(CollisionComponent &playerC, CollisionComponent const &block,
                              Utils::Velocity velocity, TILE const &collisionTile) const;
  void checkPlayerDownCollision(CollisionComponent &playerC, CollisionComponent const &block,
                                Utils::Velocity velocity, TILE const &collisionTile) const;

  void resetPlayerCollisions(CollisionComponent &collisionComponent) const;

  void updatePlayerCollisionRect(CollisionComponent &playerCollisionComponent,
                                 TransformComponent const &playerTransformComponent) const;
};

#endif
