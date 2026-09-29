#pragma once
#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include "SDL_render.h"

struct RenderInfo {
  int objTexture;
  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

struct RenderComponent {
  RenderComponent(SDL_Rect source, SDL_Rect destination, SDL_RendererFlip flip, int texture)
      : sourceRect(source), destinationRect(destination), objTextureIndex(texture), flip(flip) {}
  RenderComponent(RenderInfo info) : sourceRect(info.sourceRect),destinationRect(info.destinationRect),objTextureIndex(info.objTexture),flip(info.flip) {}

  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  int objTextureIndex;
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  RenderInfo render() const {
  RenderInfo const info = {objTextureIndex, sourceRect, destinationRect, flip};
  return info;
}

};
#endif
