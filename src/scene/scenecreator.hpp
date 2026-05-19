#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "sceneinfo.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::basic_json<>;

class SceneCreator {
public:
  static json getGameData();
  static json data;
  static SceneInfo createLevel(SDL_RWops *Handle);

private:
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

}; // namespace Level

#endif
