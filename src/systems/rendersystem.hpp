#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../scene/sceneinfo.hpp"
#include "SDL_rect.h"

class RenderSystem {
  void renderStatic(const RenderInfo &RenderInfo) const;
  void renderNonStatic(const RenderInfo &RenderInfo) const;
  bool inView(const SDL_Rect &destinationRect) const;

public:
  SceneInfo *currentLevel = nullptr;
  void update() const;
  void render() const;
};

#endif
