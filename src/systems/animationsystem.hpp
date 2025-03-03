#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"

class AnimationSystem {
public:
  void update(CollisionComponent const &collisionComponent, GravityState const &gravityState,
              TransformComponent const &transformComponent, RenderComponent &renderComponent,
              AnimationComponent &animationComponent);

  void playFrame(RenderComponent &renderComponent, int const x, int const y, int const w,
                 int const h, int const frame);
};

#endif
