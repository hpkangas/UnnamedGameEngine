
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../scene/sceneinfo.hpp"
#include "transformsystem.hpp"

class CollisionSystem {
public:
  TransformSystem *transformSystem = nullptr;

  TILE currentTile = TILE::NONE;
  Utils::Position collidedDownObjectPos = {0, 0};
  Utils::Position collidedUpObjectPos = {0, 0};

  void update(SceneInfo &currentLevel, TransformSystem &transformSystem);

  void handleDownTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                               GravityComponent const &ctrlc) const;

  void handleUpTileCollision(TransformComponent &tc, CollisionComponent const &cc,
                             GravityComponent const &ctrlc) const;
  void handleHorizontalBlockCollision(TransformComponent &tc, CollisionComponent const &cc) const;
};

#endif
