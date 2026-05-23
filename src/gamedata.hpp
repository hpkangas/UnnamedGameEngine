#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP
#include <fstream>
#include <iostream>
#include "include/json/json.hpp"

struct GameData {
  static nlohmann::json init();
};

#endif
