#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"

class AnimationSystem {
public:
  void update(ControlComponent const &controlControl, CollisionComponent const &collisionComponent,
              GRAVITY_STATE const gravityState, TransformComponent const &transformComponent,
              RenderComponent &renderComponent, AnimationComponent &animationComponent) const;

private:
  void determineAnimationState(ControlComponent const &controlComponent,
                               CollisionComponent const &collisionComponent,
                               GRAVITY_STATE gravityState,
                               TransformComponent const &transformComponent,
                               AnimationComponent &animationComponent) const;

  void playFrame(RenderComponent &renderComponent, int const x, int const y, int const w,
                 int const h, int const frame) const;
};

#endif
