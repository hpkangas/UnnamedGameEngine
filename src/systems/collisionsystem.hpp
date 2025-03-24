
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../scenecreator.hpp"

class CollisionSystem {
public:
  CollisionSystem() {}
  ~CollisionSystem() {}
  void update(CollisionComponent &playerC, TransformComponent &playerT, SceneInfo &currentLevel);
  void checkBlockCollision(CollisionComponent &playerC, TransformComponent &playerT,
                           SceneInfo &currentLevel);
  void handleDownBlockCollision(TransformComponent &tc, CollisionComponent &cc, int dif);
  void handleHorizontalBlockCollision(TransformComponent &tc, CollisionComponent &cc, int dif);
};

#endif
