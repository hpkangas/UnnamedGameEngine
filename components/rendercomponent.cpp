#include "rendercomponent.hpp"

RenderInfo RenderComponent::render() {
  RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, SDL_FLIP_NONE};
  return info;
}