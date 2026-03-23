#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/transformcomponent.hpp"

class GravitySystem {
public:
  void gravity(TransformComponent &transformComponent) const;
  void update(ControlComponent const &controlComponent, GravityComponent &gravityComponent,
              TransformComponent &transformComponent) const;
};
#endif
