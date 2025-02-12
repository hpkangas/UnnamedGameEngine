#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include "../components/animationcomponent.hpp"
#include "../components/rendercomponent.hpp"

class AnimationSystem {
public:
  void update(RenderComponent &renderComponent, AnimationComponent &animationComponent);
  void playFrame(RenderComponent &renderComponent, int x, int y, int w, int h, int frame);
};

#endif