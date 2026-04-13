#pragma once
#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include "SDL_render.h"
#include "component.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct RenderInfo {
  int objTexture;
  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

struct RenderComponent : public IComponent {
  RenderComponent(SDL_Rect source, SDL_Rect destination, SDL_RendererFlip flip, int texture, int id)
      : sourceRect(source), destinationRect(destination), objTextureIndex(texture), flip(flip) {
    parentID = id;
  }

  SDL_Rect sourceRect{};
  SDL_Rect destinationRect{};
  int objTextureIndex;
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  RenderInfo render() const;

  void to_json(json &j, const RenderComponent &rc);
  void from_json(const json &j, RenderComponent &rc);
};
#endif
