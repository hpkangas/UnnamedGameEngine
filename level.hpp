#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "components/rendercomponent.hpp"
#include "entities/block.hpp"
#include "entities/gameobject.hpp"
#include "entities/player.hpp"
#include <vector>

struct LevelInfo {
  std::vector<GameObject *> gameobjects;
  std::vector<Player *> playercharacters;
  std::vector<Block *> blocks;
  std::vector<RenderComponent *> renderables;
};

class Level {
public:
  Level() {};
  ~Level() {};
  Player *createPlayerCharacter(std::string const object, int const x, int const y);
  Block *createBlock(std::string const object, int const x, int const y);
  LevelInfo createLevel(SDL_RWops *Handle);

  Block *createBlock1(int x, int y);
  Block *createBlock2(int x, int y);
  Block *createBlock3(int x, int y);

}; // namespace Level

#endif