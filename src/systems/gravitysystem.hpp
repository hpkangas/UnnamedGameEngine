#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent) const;
  void update(CollisionComponent const &collisionComponent, GravityComponent &gravityComponent,
              TransformComponent &transformComponent) const;
};
#endif
