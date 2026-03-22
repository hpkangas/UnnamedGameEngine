#include "scenecreator.hpp"
#include "SDL_error.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "components/animationcomponent.hpp"
#include "components/collisioncomponent.hpp"
#include "components/controlcomponent.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/tilecomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_rwops.h>
#include <asm-generic/errno.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

// for convenience
SceneInfo SceneCreator::createLevel2(SDL_RWops *file) {
  using json = nlohmann::json;
  std::ifstream f("example.json");
  json data = json::parse(f);
  std::string asd = data.at("avain");
  std::cout << asd;

  SceneInfo level;
  std::string currentValue;
  char c = '0';

  std::cout << "Creating scene" << "\n";
  loadTextures();

  int y = 0;
  int x = 0;
  while (SDL_RWread(file, &c, 1, 1) == 1) {
    if (c == '\n') {
      y++;
      x = 0;
      continue;
    }
    if (c == ',') {
      std::cout << currentValue << std::endl;

      if (currentValue.substr(0, 2) == "PC") {
        createPC(128 * x, 128 * y, level);

      } else if (currentValue.substr(0, 1) == "B") {
        createBlock(currentValue, 128 * x, 128 * y, level);

      } else if (currentValue.substr(0, 2) == "LS") {
        createLadderStart(128 * x, 128 * y, level);

      } else if (currentValue.substr(0, 1) == "L") {
        createLadder(128 * x, 128 * y, level);

      } else if (currentValue == "") {
        createEmptyTile(128 * x, 128 * y, level);

      } else {
        std::cout << "Error: Scene creator: " << currentValue << "is unknown. Line " << y << " "
                  << x << std::endl;
      }

      x++;
      currentValue = "";
      continue;
    }

    else {
      currentValue += c;
    }
  }
  SDL_RWclose(file);

  return level;
}
SceneInfo SceneCreator::createLevel(SDL_RWops *file) {
  SceneInfo level;
  std::string currentValue;
  char c;
  std::vector<std::string> values;

  std::cout << "Creating scene" << "\n";
  loadTextures();

  while (SDL_RWread(file, &c, 1, 1) == 1) {
    // Read value before comma. Create stuff from values at new line

    if (c == '\n') {
      std::string gameObjectType = values.at(0);
      int x = std::stoi(values.at(1));
      int y = std::stoi(values.at(2));

      if (gameObjectType.substr(0, 2) == "PC") {
        createPC(x, y, level);

      } else if (gameObjectType.substr(0, 1) == "B") {
        createBlock(gameObjectType, x, y, level);

      } else if (gameObjectType.substr(0, 2) == "LS") {
        createLadderStart(x, y, level);

      } else if (gameObjectType.substr(0, 1) == "L") {
        createLadder(x, y, level);
      } else {
        std::cout << "Error: Scene creator: " << gameObjectType << "is unknown" << std::endl;
      }

      values.clear();
      continue;
    }

    if (c != ',') {
      currentValue += c;
    } else {
      values.emplace_back(currentValue);
      currentValue = "";
    }
  }
  SDL_RWclose(file);
  return level;
}

void SceneCreator::createBlock(std::string gameObjectType, int x, int y, SceneInfo &level) {
  if (gameObjectType == "B1") {
    createBlock1(x, y, level);
  } else if (gameObjectType == "B2") {
    createBlock2(x, y, level);
  } else if (gameObjectType == "B3") {
    createBlock3(x, y, level);
  } else {
    std::cout << "Scene creator: couldn't create block " << gameObjectType << std::endl;
  }
}

void SceneCreator::createBlock1(int x, int y, SceneInfo &level) {
  createBlockEntity(x, y, 0, level);
}
void SceneCreator::createBlock2(int x, int y, SceneInfo &level) {
  createBlockEntity(x, y, 128, level);
}
void SceneCreator::createBlock3(int x, int y, SceneInfo &level) {
  createBlockEntity(x, y, 256, level);
}
void SceneCreator::createEmptyTile(int x, int y, SceneInfo &level) {
  int id = getId();

  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  transformComponent->parentID = id;

  auto tileComponent = std::make_unique<TileComponent>(TILE::NONE);

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::createBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto collisionComponent = std::make_unique<CollisionComponent>(destinationRect, id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::SOLID);

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.renderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto collisionComponent = std::make_unique<CollisionComponent>(sourceRect, id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::SEMISOLID);

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.renderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::loadTextures() {
  std::cout << "Loading textures...\n";
  textures.emplace_back(TextureManager::LoadTexture("assets/baleog.png"));
  textures.emplace_back(TextureManager::LoadTexture("assets/tiles.png"));
}

void SceneCreator::destroyTextures() {
  for (auto &tex : textures) {
    SDL_DestroyTexture(tex);
  }
}

void SceneCreator::createPC(int x, int y, SceneInfo &level) {
  int id = getId();

  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(0), id);
  SDL_Rect collisionRect = {transformComponent->getXpos(), transformComponent->getYpos(),
                            PLAYER_WIDTH, PLAYER_HEIGHT};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);
  auto gravityComponent = std::make_unique<GravityComponent>(GRAVITY_STATE::AIRBORNE, id);
  auto animationComponent = std::make_unique<AnimationComponent>();
  auto controlComponent = std::make_unique<ControlComponent>(CONTROL_STATE::AIRBORNE, id);

  animationComponent->parentID = id;

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.gcs.emplace(id, (std::move(gravityComponent)));
  level.acs.emplace(id, (std::move(animationComponent)));
  level.ctrlComponents.emplace(id, (std::move(controlComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.renderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.gravityIds.emplace_back(id);
  level.tileIds.emplace_back(id);
  level.controlIds.emplace_back(id);
  level.animationIds.emplace_back(id);
}

void SceneCreator::createLadder(int x, int y, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 192, 128, 128};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::LADDER);
  SDL_Rect collisionRect = {x, y, 128, 128};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.renderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::createLadderStart(int x, int y, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 192, 128, 64};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::LADDERSTART);
  SDL_Rect collisionRect = {x, y, 128, 64};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.renderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

int SceneCreator::getId() {
  static int id = -1;
  id++;
  return id;
}
