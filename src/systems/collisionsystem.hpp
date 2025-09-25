
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../scenecreator.hpp"
#include "transformsystem.hpp"

class CollisionSystem {
public:
  CollisionSystem() {}
  ~CollisionSystem() {}

  TransformSystem *transformSystem;

  TILE currentTile = TILE::NONE;
  Position collidedDownObjectPos = {0, 0};
  Position collidedUpObjectPos = {0, 0};

  void update(int i, SceneInfo &currentLevel, TransformSystem &transformSystem);

  void handleDownTileCollision(TransformComponent &playerTransformComponent,
                               CollisionComponent &playerCollisionComponent,
                               ControlComponent &playerControlComponent);

  void handleUpTileCollision(TransformComponent &playerTransformComponent,
                             CollisionComponent &playerCollisionComponent,
                             ControlComponent &playerControlComponent);
  void handleHorizontalBlockCollision(TransformComponent &tc, CollisionComponent &cc);
};

#endif
