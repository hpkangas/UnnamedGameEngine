#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class RenderSystem {
public:
  RenderSystem() {}
  ~RenderSystem() {}

  void renderComponents(std::unordered_map<int, std::unique_ptr<RenderComponent>> &components,
                        std::vector<int> renderIds);
  void render(RenderComponent &renderComponent);
  void update(const TransformComponent &tc, RenderComponent &rc);
  void draw(const RenderInfo &RenderInfo);
};

#endif
