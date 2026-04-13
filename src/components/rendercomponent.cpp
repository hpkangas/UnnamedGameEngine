#include "rendercomponent.hpp"

RenderInfo RenderComponent::render() const {
  RenderInfo const info = {objTextureIndex, sourceRect, destinationRect, flip};
  return info;
}

void RenderComponent::to_json(json &j, const RenderComponent &rc) {}
void RenderComponent::from_json(const json &j, RenderComponent &rc) {}
