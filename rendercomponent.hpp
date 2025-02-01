#pragma once
#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP
#include "SDL_render.h"

struct RenderInfo {
  SDL_Texture *objTexture{};
  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

struct RenderComponent {

  SDL_Texture *m_objTexture{};
  SDL_Rect m_sourceRect{}, m_destinationRect{};
  RenderInfo render();
  RenderComponent() {}
  ~RenderComponent() {}
};
#endif