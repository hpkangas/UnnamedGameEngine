#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../scene/scenecreator.hpp"

class RenderSystem {
public:
  void renderComponents(SceneInfo &currentLevel) const;
  void render(const RenderInfo &RenderInfo) const;
  void update(SceneInfo &currentLevel) const;
  void renderWithCamera(const RenderInfo &RenderInfo) const;
};

#endif
