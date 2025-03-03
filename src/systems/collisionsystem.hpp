
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../scenecreator.hpp"

class CollisionSystem {
public:
  CollisionSystem() {}
  ~CollisionSystem() {}
  void update(CollisionComponent &playerC, TransformComponent &playerT, SceneInfo &currentLevel);
  void handleDownCollision(TransformComponent &tc, CollisionComponent &cc);
  void handleHorizontalCollision(TransformComponent &tc, CollisionComponent &cc);
};

#endif
