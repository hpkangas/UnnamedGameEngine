#pragma once
#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include "SDL_render.h"
#include "component.hpp"

struct RenderInfo {
  SDL_Texture *objTexture{};
  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

struct RenderComponent : public IComponent {
  RenderComponent() {}
  ~RenderComponent() {
    SDL_DestroyTexture(objTexture);
  }

  int ID = RCID;

  SDL_Texture *objTexture{};
  SDL_Rect sourceRect{}, destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  RenderInfo const render();
};
#endif
