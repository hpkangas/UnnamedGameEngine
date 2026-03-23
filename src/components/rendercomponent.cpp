#include "rendercomponent.hpp"

RenderInfo RenderComponent::render() {
  RenderInfo const info = {objTexture, sourceRect, destinationRect, flip};
  return info;
}
