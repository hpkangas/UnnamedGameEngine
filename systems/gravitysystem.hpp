#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent);
  void update(GravityComponent &gravityComponent, AnimationComponent &animationComponent,
              TransformComponent &transformComponent);
};
#endif