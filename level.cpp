#include "level.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <iostream>
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
      // tällä saa esim etuliitteen
        std::cout<<currentValue.substr(0,2)<<std::endl;
      if (currentValue != "|") {

        values.emplace_back(currentValue);

      } else {
        std::string gameobject = values.at(0);
        int x = std::stoi(values.at(1));
        int y = std::stoi(values.at(2));

        // pitänee olla iffittely eri oliotyypeille ja omat funktiot niille

        // some other fucn
//        GameObject *go = createGameObject(gameobject, x, y);
        // level->gameobjects->emplace_back(createGameObject(gameobject, x, y));
        
        values.clear();
      }

      currentValue = "";
      continue;
    }

    currentValue += c;
  }
  return level;
};

GameObject *createGameObject(std::string object, int x, int y) {
  // GameObject *gameobject = nullptr;

  // if (object == "PC1") {
  //   gameobject = new Player(x, y, "Erik");
  //   return gameobject;
  // }
  // if (object == "PC2") {
  //   gameobject = new Player(x, y, "Baelog");
  //   return gameobject;
  // }
  // if (object == "PC2") {
  //   gameobject = new Player(x, y, "Olaf");
  //   return gameobject;
  // }

   return nullptr;
};

} // namespace Level