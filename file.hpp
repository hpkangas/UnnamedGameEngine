#include "player.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <iostream>
#include <memory>
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

std::string ReadString(SDL_RWops *Handle) {
  std::string CurrentValue;
  char c;

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {
    std::cout << "c " << c << std::endl;

    if (c == ',') {
      std::cout << "current value " << CurrentValue << std::endl;
      return CurrentValue;
    }
    CurrentValue += c;
  }
  return CurrentValue;
}

int ReadInt(SDL_RWops *Handle) { return std::stoi(ReadString(Handle)); }

float ReadFloat(SDL_RWops *Handle) { return std::stof(ReadString(Handle)); }

std::vector<Player *> players;

void createLevel(SDL_RWops *Handle) {
  std::string CurrentValue;
  std::string Target;
  int x;
  int y;
  char c;

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {
    Player *asd = nullptr;

    if (c == ',') {
      std::cout << "comma" << std::endl;
      std::cout << "current value " << CurrentValue << "\n";

      if (CurrentValue == "Erik" || CurrentValue == "Baleog" || CurrentValue == "Olaf") {
        Target = CurrentValue;
      }

      std::string apu = &CurrentValue.at(0);
      if (apu == "x") {
        x = std::stoi(CurrentValue);
        std::cout<<x;
      }
      if (apu == "y") {
        y = std::stoi(CurrentValue);
        std::cout<<y;
      }

      CurrentValue = "";
      continue;
    }

    if (c == '|') {

      asd = new Player(x, y, CurrentValue);
      players.emplace_back(asd);
      continue;
    }

    CurrentValue += c;
  }
}

} // namespace File