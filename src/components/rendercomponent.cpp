#include "rendercomponent.hpp"

RenderInfo RenderComponent::render() {
  RenderInfo const info = {objTexture, sourceRect, destinationRect, flip};
  return info;
}

void RenderComponent::to_json(json &j, const RenderComponent &rc){
};
void RenderComponent::from_json(const json &j, RenderComponent &rc){};
