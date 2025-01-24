#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "gameobject.hpp"
#include "player.hpp"
#include <vector>

namespace Level {

struct LevelInfo {
  std::vector<GameObject *> *gameobjects = nullptr;
  std::vector<Player *> *playercharacters = nullptr;
};

GameObject *createGameObject(std::string object, int x, int y);
LevelInfo *createLevel(SDL_RWops *Handle);
} // namespace Level

#endif