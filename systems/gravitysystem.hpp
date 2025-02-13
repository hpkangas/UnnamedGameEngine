#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../entities/entity.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent);
  void update(Entity &selectedPlayer, CollisionComponent &collisionComponent,
              GravityComponent &gravityComponent, AnimationComponent &animationComponent,
              TransformComponent &transformComponent);
};
#endif