#include "level.hpp"
#include "block.hpp"
#include "player.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <string>
#include <vector>

namespace Level {

LevelInfo *createLevel(SDL_RWops *Handle) {
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
          level->playercharacters.emplace_back(createPlayerCharacter(gameObjectType, x, y));
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

Player *createPlayerCharacter(std::string gameObjectType, int x, int y) {
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

Block *createBlock(std::string gameObjectType, int x, int y){
  // pitää olla omat constructorit eri blockeille joille nyt on vaan parametrit
  return nullptr;
}

} // namespace Level