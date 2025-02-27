#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../entities/entity.hpp"
#include <vector>

class RenderSystem {
public:
  RenderSystem() {}
  ~RenderSystem() {}

  void renderComponents(std::vector<RenderComponent *> renderables);
  void render(RenderComponent &renderComponent);
  void update(Entity &player);
  void draw(const RenderInfo &RenderInfo);
};

#endif
