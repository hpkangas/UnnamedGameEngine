#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class RenderSystem {
public:
  void renderComponents(std::unordered_map<int, std::unique_ptr<RenderComponent>> &components,
                        std::vector<int> const &renderIds) const;
  void render(RenderComponent &renderComponent) const;
  void update(const TransformComponent &tc, RenderComponent &rc) const;
  void draw(const RenderInfo &RenderInfo) const;
};

#endif
