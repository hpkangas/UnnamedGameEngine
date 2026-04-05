#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "SDL_render.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

using json = nlohmann::basic_json<>;

struct SceneInfo {
  std::unordered_map<int, std::unique_ptr<TransformComponent>> tcs;
  std::unordered_map<int, std::unique_ptr<RenderComponent>> rcs;
  std::unordered_map<int, std::unique_ptr<CollisionComponent>> ccs;
  std::unordered_map<int, std::unique_ptr<GravityComponent>> gcs;
  std::unordered_map<int, std::unique_ptr<AnimationComponent>> acs;
  std::unordered_map<int, std::unique_ptr<TileComponent>> tiles;
  std::unordered_map<int, std::unique_ptr<ControlComponent>> ctrlComponents;
  std::unordered_map<int, std::unique_ptr<HealthComponent>> healthComponents;
  std::vector<int> transformIds;
  std::vector<int> ObjectRenderIds;
  std::vector<int> UIRenderIds;
  std::vector<int> TileRenderIds;
  std::vector<int> collisionIds;
  std::vector<int> gravityIds;
  std::vector<int> animationIds;
  std::vector<int> tileIds;
  std::vector<int> controlIds;
  std::vector<int> healthIds;
  int ids = 0;
};

class SceneCreator {
public:
  static json getGameData();
  static json data;
  static SceneInfo createLevel(SDL_RWops *Handle);

private:
  inline static std::vector<SDL_Texture *> textures;

  static void createBlock(std::string const &object, int const x, int const y, SceneInfo &level);
  static void createBlock1(int x, int y, SceneInfo &level);
  static void createBlock2(int x, int y, SceneInfo &level);
  static void createBlock3(int x, int y, SceneInfo &level);
  static void createEmptyTile(int x, int y, SceneInfo &level);
  static void createBlockEntity(int x, int y, int sourceX, SceneInfo &level);
  static void createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level);
  static void createLadder(int x, int y, SceneInfo &level);
  static void createLadderStart(int x, int y, SceneInfo &level);
  static void createPC(int x, int y, SceneInfo &level);
  static void createEnemy(int x, int y, SceneInfo &level);
  static void createUI(int x, int y, SceneInfo &level);

  static void loadTextures();
  void destroyTextures() const;

  static int getId();

}; // namespace Level

#endif
