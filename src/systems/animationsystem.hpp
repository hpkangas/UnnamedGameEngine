#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/shootcomponent.hpp"
#include "../scene/sceneinfo.hpp"

class AnimationSystem {
public:
  void update();
  SceneInfo *currentLevel = nullptr;

private:
  void resetAnimation(AnimationComponent &animationComponent) const;

  void determineAnimationState(AnimationComponent &animationComponent, entt::entity entity) const;

  void playFrame(RenderComponent &renderComponent, int const x, int const y, int const w,
                 int const h, int const frame) const;

  void animate(AnimationComponent &animationComponent, entt::entity entity) const;
};

#endif
