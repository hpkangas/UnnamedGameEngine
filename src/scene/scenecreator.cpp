#include "scenecreator.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../include/json/json.hpp"
#include "entitycreator.hpp"
#include <SDL2/SDL_rwops.h>
#include "../utilities/constants.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

SceneInfo SceneCreator::createLevel() {
  SceneInfo level;

  static nlohmann::json levelJson;

  levelJson = getLevelJson("data/level.json");
  createEntitiesFromJson(levelJson, level);

  return level;
}

nlohmann::json SceneCreator::getLevelJson(std::string const &filePath) {
  static nlohmann::json levelJson;

  std::ifstream ifs(filePath);
  if (!ifs.is_open()) {
    std::cout << "SceneCreator: failed to open " << filePath << "\n";
  }
  try {
    ifs >> levelJson;
  } catch (const std::exception &e) {
    std::cout << "SceneCreator: json parse error: " << e.what() << std::endl;
    levelJson = nlohmann::json(); // empty
  }
  ifs.close();
  return levelJson;
}

void SceneCreator::createEntitiesFromJson(nlohmann::json const &levelJson, SceneInfo &level) {
  int x = 0;
  int y = 0;

  std::cout << "Creating scene" << "\n";
  auto tiles = levelJson["tiles"];
  for (auto row : tiles.items()) {
    y++;
    x = 0;
    for (auto col : row.value().items()) {
      if (col.value() != "") {
        createTileFromJson(col.value(), x, y, level);
      }
      x++;
    }
  }

  level.levelLength = x;
  level.levelHeight = y;

  auto entities = levelJson["entities"];
  for (auto entity : entities.items()) {
    std::string type = entity.value().value("type", "");
    int posX = entity.value().value("x", 0);
    int posY = entity.value().value("y", 0);
    createTileFromJson(type, posX, posY, level);
  }

  editTileGrid(level);
}

void SceneCreator::createTileFromJson(std::string currentValue, int x, int y, SceneInfo &level) {
  EntityCreator::createEntityOnTilePosition(currentValue, Position{x, y}, level);
}

void SceneCreator::editTileGrid(SceneInfo &level) {
  // Bake the static-collider tile grid for O(1) neighbour queries.
  level.staticTileGrid.resize(level.levelLength, level.levelHeight + 1);
  auto tileView = level.registry.view<TileComponent>();
  for (entt::entity tileEntity : tileView) {
    TileComponent const &tile = level.registry.get<TileComponent>(tileEntity);
    PositionComponent const *position =
        level.registry.try_get<PositionComponent>(tileEntity);
    CollisionComponent const *collision =
        level.registry.try_get<CollisionComponent>(tileEntity);
    if (position && collision) {
      level.staticTileGrid.addRectangle(position->position.x, position->position.y,
                                        collision->collisionRect.w, collision->collisionRect.h,
                                        Constants::TILE_SIZE, tileEntity);
    } else {
      level.staticTileGrid.add(tile.tilePosition.x, tile.tilePosition.y, tileEntity);
    }
  }
}
