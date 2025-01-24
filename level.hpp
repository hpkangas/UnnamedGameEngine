#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "block.hpp"
#include "gameobject.hpp"
#include "player.hpp"
#include <vector>

namespace Level {

struct LevelInfo {
  std::vector<GameObject *> gameobjects;
  std::vector<Player *> playercharacters;
  std::vector<Block *> blocks;
};

Player *createPlayerCharacter(std::string const object, int const x, int const y);
Block *createBlock(std::string const object, int const x, int const y);
LevelInfo *createLevel(SDL_RWops *Handle);
} // namespace Level

#endif