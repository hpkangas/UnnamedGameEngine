#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../entities/player.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent);
  void update(Player &selectedPlayer,CollisionComponent &collisionComponent, GravityComponent &gravityComponent,
              AnimationComponent &animationComponent, TransformComponent &transformComponent);
};
#endif