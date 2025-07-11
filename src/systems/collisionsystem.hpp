
#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include "../scenecreator.hpp"

class CollisionSystem {
public:
  CollisionSystem() {
    yposDif = 0;
    xposDif = 0;
  }
  ~CollisionSystem() {}
  int yposDif;
  int xposDif;
  void update(CollisionComponent &playerC, TransformComponent &playerT, SceneInfo &currentLevel);
  void checkBlockCollision(CollisionComponent &playerC, TransformComponent &playerT,
                           SceneInfo &currentLevel);

  void checkPlayerLeftCollision(CollisionComponent &playerC, CollisionComponent &block, Velocity velocity);
  void checkPlayerRightCollision(CollisionComponent &playerC, CollisionComponent &block, Velocity velocity);
  void checkPlayerUpCollision(CollisionComponent &playerC, CollisionComponent &block, Velocity velocity);
  void checkPlayerDownCollision(CollisionComponent &playerC, CollisionComponent &block, Velocity velocity);


  void handleVerticalBlockCollision(TransformComponent &tc, CollisionComponent &cc, int dif);
  void handleHorizontalBlockCollision(TransformComponent &tc, CollisionComponent &cc, int dif);
};

#endif
