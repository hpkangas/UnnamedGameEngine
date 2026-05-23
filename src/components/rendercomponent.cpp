#include "rendercomponent.hpp"

RenderInfo RenderComponent::render() const {
  RenderInfo const info = {objTextureIndex, sourceRect, destinationRect, flip};
  return info;
}

