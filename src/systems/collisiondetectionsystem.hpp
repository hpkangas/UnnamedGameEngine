
#ifndef COLLISIONDETECTIONSYSTEM_HPP
#define COLLISIONDETECTIONSYSTEM_HPP
#include "../scenecreator.hpp"

class CollisionDetectionSystem {
public:
  CollisionDetectionSystem() {}
  ~CollisionDetectionSystem() {}

  TILE currentTile = TILE::NONE;
  Position collidedDownObjectPos = {0, 0};
  Position collidedUpObjectPos = {0, 0};

  void update(int i, SceneInfo &currentLevel);
  void checkBlockCollision(int i, SceneInfo &currentLevel);

  void checkPlayerLeftCollision(CollisionComponent &playerC, CollisionComponent &block,
                                Velocity velocity, TILE &collisionTile);
  void checkPlayerRightCollision(CollisionComponent &playerC, CollisionComponent &block,
                                 Velocity velocity, TILE &collisionTile);
  void checkPlayerUpCollision(CollisionComponent &playerC, CollisionComponent &block,
                              Velocity velocity, TILE &collisionTile);
  void checkPlayerDownCollision(CollisionComponent &playerC, CollisionComponent &block,
                                Velocity velocity, TILE &collisionTile);

  void resetPlayerCollisions(CollisionComponent &collisionComponent);

  void updatePlayerCollisionRect(CollisionComponent &playerCollisionComponent,
                                 TransformComponent &playerTransformComponent);
};

#endif
