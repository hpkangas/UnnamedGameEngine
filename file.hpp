#include "player.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

enum gamo {
  ERIK = 0,
  BALEOG = 1,
  OLAF = 2

};

namespace File {
inline void Read(const std::string &Path) {
  char *Content{static_cast<char *>(SDL_LoadFile(Path.c_str(), nullptr))};

  if (Content) {
    std::cout << "Content: " << Content;
  } else {
    std::cout << "Error loading file: " << SDL_GetError();
  }

  SDL_free(Content);
}

std::vector<Player *> players;

void createLevel(SDL_RWops *Handle) {
  std::string currentValue;
  std::string gameobject;
  int x;
  int y;
  char c;

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {
    Player *player = nullptr;

    if (c == ',') {

      if (currentValue == "Erik" || currentValue == "Baleog" || currentValue == "Olaf") {
        gameobject = currentValue;
      }

      if (currentValue[0] == 'x') {
        x = std::stoi(currentValue.substr(2));
      }
      if (currentValue[0] == 'y') {
        y = std::stoi(currentValue.substr(2));
      }

      if (currentValue == "|") {

        player = new Player(x, y, gameobject);
        players.emplace_back(player);
        continue;
      }

      currentValue = "";
      continue;
    }

    currentValue += c;
  }
}

} // namespace File