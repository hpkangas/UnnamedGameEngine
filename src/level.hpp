#pragma once
#include "components/animationcomponent.hpp"
#include "components/collisioncomponent.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/transformcomponent.hpp"
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "SDL_render.h"
#include "components/component.hpp"
#include "entities/entity.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

using ComponentVector = std::vector<std::unique_ptr<IComponent>>;
using EntityVector = std::vector<std::unique_ptr<Entity>>;

struct LevelInfo {
  EntityVector entities;
  std::unordered_map<int, std::unique_ptr<IComponent>> components;
  std::unordered_map<int, std::unique_ptr<TransformComponent>> tcs;
  std::unordered_map<int, std::unique_ptr<RenderComponent>> rcs;
  std::unordered_map<int, std::unique_ptr<CollisionComponent>> ccs;
  std::unordered_map<int, std::unique_ptr<GravityComponent>> gcs;
  std::unordered_map<int, std::unique_ptr<AnimationComponent>> acs;
  int ids = 0;
};

class Level {
public:
  Level() {}
  ~Level() {}
  std::vector<SDL_Texture *> textures;

  LevelInfo createLevel(SDL_RWops *Handle);
  std::unique_ptr<Entity> createBlock(std::string const object, int const x, int const y);
  std::unique_ptr<Entity> createBlock1(int x, int y);
  std::unique_ptr<Entity> createBlock2(int x, int y);
  std::unique_ptr<Entity> createBlock3(int x, int y);
  std::unique_ptr<Entity> createBlockEntity(int x, int y, int sourceX);
  std::unique_ptr<Entity> createPCEntity(int x, int y);
  void loadTextures();
  void destroyTextures();
  void createPC(int x, int y, LevelInfo &level);
  int getID();

}; // namespace Level

#endif
