#include "rendercomponent.hpp"

RenderInfo const RenderComponent::render() {
  RenderInfo const info = {objTexture, sourceRect, destinationRect, flip};
  return info;
}
