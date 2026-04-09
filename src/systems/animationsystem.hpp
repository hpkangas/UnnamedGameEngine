#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../scene/sceneinfo.hpp"

class AnimationSystem {
public:
SceneInfo *currentLevel = nullptr;
  void update() const;

private:
  void determineAnimationState(CollisionComponent const &collisionComponent,
                               GRAVITY_STATE const gravityState,
                               TransformComponent const &transformComponent,
                               AnimationComponent &animationComponent) const;

  void playFrame(RenderComponent &renderComponent, int const x, int const y, int const w,
                 int const h, int const frame) const;

  void animate(RenderComponent &renderComponent, AnimationComponent &animationComponent,TransformComponent const &transformComponent) const;
};

#endif
