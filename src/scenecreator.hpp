#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "SDL_render.h"
#include "components/animationcomponent.hpp"
#include "components/collisioncomponent.hpp"
#include "components/controlcomponent.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/tilecomponent.hpp"
#include "components/transformcomponent.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

struct SceneInfo {
  std::unordered_map<int, std::unique_ptr<TransformComponent>> tcs;
  std::unordered_map<int, std::unique_ptr<RenderComponent>> rcs;
  std::unordered_map<int, std::unique_ptr<CollisionComponent>> ccs;
  std::unordered_map<int, std::unique_ptr<GravityComponent>> gcs;
  std::unordered_map<int, std::unique_ptr<AnimationComponent>> acs;
  std::unordered_map<int, std::unique_ptr<TileComponent>> tiles;
  std::unordered_map<int, std::unique_ptr<ControlComponent>> controlComponents;
  int ids = 0;
};

class SceneCreator {
public:
  SceneCreator() {}
  ~SceneCreator() {}
  inline static std::vector<SDL_Texture *> textures;

  static SceneInfo createLevel(SDL_RWops *Handle);
  static void createBlock(std::string const object, int const x, int const y, SceneInfo &level);
  static void createBlock1(int x, int y, SceneInfo &level);
  static void createBlock2(int x, int y, SceneInfo &level);
  static void createBlock3(int x, int y, SceneInfo &level);
  static void createBlockEntity(int x, int y, int sourceX, SceneInfo &level);
  static void createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level);
  static void createLadder(int x, int y, SceneInfo &level);
  static void createLadderStart(int x, int y, SceneInfo &level);
  static void loadTextures();
  void destroyTextures();
  static void createPC(int x, int y, SceneInfo &level);
  static int getID();

}; // namespace Level

#endif
