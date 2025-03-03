#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent);
  void update(bool collisionDown, GravityComponent &gravityComponent,
              TransformComponent &transformComponent);
};
#endif
