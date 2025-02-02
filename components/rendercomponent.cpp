#include "rendercomponent.hpp"

RenderInfo RenderComponent::render() {
  RenderInfo info = {objTexture, sourceRect, destinationRect, flip};
  return info;
}