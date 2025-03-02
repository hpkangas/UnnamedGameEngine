
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../level.hpp"

class CollisionSystem {
public:
  CollisionSystem() {}
  ~CollisionSystem() {}
  void update(CollisionComponent &playerC, TransformComponent &playerT, LevelInfo &currentLevel);
  void handleDownCollision(Entity &selectedPlayer);
  void handleHorizontalCollision(Entity &selectedPlayer);
};

#endif
