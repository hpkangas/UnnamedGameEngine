#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent);
  void update(ControlComponent &controlComponent, GravityComponent &gravityComponent,
              TransformComponent &transformComponent);
};
#endif
