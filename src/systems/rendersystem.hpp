#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <memory>
#include <unordered_map>

class RenderSystem {
public:
  RenderSystem() {}
  ~RenderSystem() {}

  std::unordered_map<int, RenderComponent *> asd;

  void renderComponents(std::unordered_map<int, std::unique_ptr<RenderComponent>> &components);
  void render(RenderComponent &renderComponent);
  void update(const TransformComponent &tc, RenderComponent &rc);
  void draw(const RenderInfo &RenderInfo);
};

#endif
