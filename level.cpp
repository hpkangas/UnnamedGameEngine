#include "level.hpp"
#include "entities/block.hpp"
#include "entities/player.hpp"
#include <SDL2/SDL_rwops.h>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

LevelInfo Level::createLevel(SDL_RWops *Handle) {
  LevelInfo level;
  std::string currentValue;
  char c;
  std::vector<std::string> values;

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {
    if (c == '\n') {
      continue;
    }

    if (c == ',') {

      if (currentValue != "|") {

        values.emplace_back(currentValue);

      } else {
        std::string gameObjectType = values.at(0);
        int x = std::stoi(values.at(1));
        int y = std::stoi(values.at(2));

        if (gameObjectType.substr(0, 2) == "PC") {
          Player *player = createPlayerCharacter(gameObjectType, x, y);
          level.playercharacters.emplace_back(player);
        }

        if (gameObjectType.substr(0, 1) == "B") {
          Block *block = createBlock(gameObjectType, x, y);
          level.blocks.emplace_back(block);
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
  // Player player;

  if (gameObjectType == "PC1") {
    return new Player(x, y, "Erik");
  }
  if (gameObjectType == "PC2") {
    return new Player(x, y, "Olaf");
  }
  if (gameObjectType == "PC3") {
    return new Player(x, y, "Baleog");
  }
  return new Player(0, 0, "NOT REAL");
}
Block *Level::createBlock1(int x, int y) { return new Block(x, y, 0); }
Block *Level::createBlock2(int x, int y) { return new Block(x, y, 128); }
Block *Level::createBlock3(int x, int y) { return new Block(x, y, 256); }

Block *Level::createBlock(std::string gameObjectType, int x, int y) {
  if (gameObjectType == "B1") {
    return createBlock1(x, y);
  }
  if (gameObjectType == "B2") {
    return createBlock2(x, y);
  }
  if (gameObjectType == "B3") {
    return createBlock3(x, y);
  }
  return createBlock1(0, 0);
}
