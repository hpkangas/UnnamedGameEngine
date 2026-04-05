#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../scene/scenecreator.hpp"
#include "SDL_rect.h"

class RenderSystem {
  void renderStatic(const RenderInfo &RenderInfo) const;
  void renderNonStatic(const RenderInfo &RenderInfo) const;
  bool inView(const SDL_Rect &destinationRect) const;

public:
  void update(SceneInfo &currentLevel) const;
  void render(SceneInfo &currentLevel) const;
};

#endif
