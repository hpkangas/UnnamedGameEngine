
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../level.hpp"

class CollisionSystem {
public:
  CollisionSystem(){};
  ~CollisionSystem(){};
  void update(Player &selectedPlayer, LevelInfo &currentLevel);
  void handleDownCollision(Player &selectedPlayer);
  void handleHorizontalCollision(Player &selectedPlayer);
};

#endif