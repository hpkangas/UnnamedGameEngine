#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/collisioncomponent.hpp"

class AnimationSystem {
public:
  void update(CollisionComponent &collisionComponent, GravityState const &gravityState, TransformComponent &transformComponent, RenderComponent &renderComponent, AnimationComponent &animationComponent);
  void playFrame(RenderComponent &renderComponent, int x, int y, int w, int h, int frame);
};

#endif