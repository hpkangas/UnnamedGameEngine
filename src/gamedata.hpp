#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

struct GameData {
  static nlohmann::json init();
};

#endif
