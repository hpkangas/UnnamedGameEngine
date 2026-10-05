#pragma once
#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include "SDL_render.h"
#include "../include/json/json.hpp"

enum class RENDER_LAYER {
  PLAYER_CHARACTER = 0,
  PLAYER_PROJECTILE = 1,
  ENEMY_CHARACTER = 2,
  TILE = 3
};

NLOHMANN_JSON_SERIALIZE_ENUM(RENDER_LAYER, {
  {RENDER_LAYER::PLAYER_CHARACTER, "PLAYER_CHARACTER"},
  {RENDER_LAYER::PLAYER_PROJECTILE, "PLAYER_PROJECTILE"},
  {RENDER_LAYER::ENEMY_CHARACTER, "ENEMY_CHARACTER"},
  {RENDER_LAYER::TILE, "TILE"},
})

struct RenderInfo {
  int objTexture;
  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

struct RenderComponent {
  RenderComponent(SDL_Rect source, SDL_Rect destination, SDL_RendererFlip flip, int texture, RENDER_LAYER layer)
      : sourceRect(source), destinationRect(destination), objTextureIndex(texture), flip(flip),renderLayer(layer) {}

  RenderComponent(RenderInfo info) : sourceRect(info.sourceRect),destinationRect(info.destinationRect),objTextureIndex(info.objTexture),flip(info.flip) {}

  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  int objTextureIndex;
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  RENDER_LAYER renderLayer = RENDER_LAYER::PLAYER_CHARACTER;

  RenderInfo render() const {
  RenderInfo const info = {objTextureIndex, sourceRect, destinationRect, flip};
  return info;
}

};
#endif
