#pragma once
#ifndef SCENECREATOR_HPP
#define SCENECREATOR_HPP
#include "SDL_rwops.h"
#include "sceneinfo.hpp"
#include "../include/json/json.hpp"

namespace SceneCreator {
SceneInfo createLevel(SDL_RWops *Handle);
void createTileFromJson(std::string currentValue, int x, int y, SceneInfo &level);
nlohmann::json getLevelJson(std::string const &filePath);
void createEntitiesFromJson(nlohmann::json const &levelJson, SceneInfo &level);
void editTileGrid(SceneInfo &level);

}
#endif
