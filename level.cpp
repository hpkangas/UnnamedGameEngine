#include "level.hpp"
#include "block.hpp"
#include "player.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <string>
#include <vector>

LevelInfo *Level::createLevel(SDL_RWops *Handle) {
  LevelInfo *level = new LevelInfo;
  std::string currentValue;
  char c;
  std::vector<std::string> values;

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {

    if (c == ',') {

      if (currentValue != "|") {

        values.emplace_back(currentValue);

      } else {
        std::string gameObjectType = values.at(0);
        int x = std::stoi(values.at(1));
        int y = std::stoi(values.at(2));

        if (gameObjectType.substr(0, 2) == "PC") {
          level->playercharacters.emplace_back(createPlayerCharacter(gameObjectType, x, y));
        }

        if (gameObjectType.substr(0, 1) == "B") {
          level->blocks.emplace_back(createBlock(gameObjectType, x, y));
        }

        values.clear();
      }
      currentValue = "";
      continue;
    }

    currentValue += c;
  }
  return level;
}

Player *Level::createPlayerCharacter(std::string gameObjectType, int x, int y) {
  // some general creator function here too rather than calling constructor straight away?
  Player *player = nullptr;
  
  if (gameObjectType == "PC1") {
    player = new Player(x, y, "Erik");
    return player;
  }
  if (gameObjectType == "PC2") {
    player = new Player(x, y, "Olaf");
    return player;
  }
  if (gameObjectType == "PC3") {
    player = new Player(x, y, "Baleog");
    return player;
  }
  player = new Player(0, 0, "NOT REAL");
  return player;
}

Block *Level::createBlock(std::string gameObjectType, const int x, const int y) {
  if (gameObjectType == "B1") {
    return createBlock1(x, y);
  }
  if (gameObjectType == "B2") {
    return createBlock2(x, y);
  }
  if (gameObjectType == "B3") {
    return createBlock3(x, y);
  }
  return nullptr;
}

Block *Level::createBlock1(const int x, const int y) { return new Block(x, y, 0); }
Block *Level::createBlock2(const int x, const int y) { return new Block(x, y, 128); }
Block *Level::createBlock3(const int x, const int y) { return new Block(x, y, 256); }
