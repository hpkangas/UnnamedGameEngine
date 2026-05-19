#include "scenecreator.hpp"
#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../gamedata.hpp"
#include "../texturemanager.hpp"
#include "../utilities/constants.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>

using namespace Constants;

SceneInfo SceneCreator::createLevel(SDL_RWops *file) {
  SceneInfo level;

  // Section 2.4: pre-size hot pools so the bulk of the level load does
  // not trigger rehashes. Sizes are generous upper bounds for the kind
  // of level shipped with the project.
  level.aiComponents.reserve(64);
  level.animationComponents.reserve(64);
  level.attackComponents.reserve(64);
  level.collisionComponents.reserve(2048);
  level.controlComponents.reserve(8);
  level.healthComponents.reserve(64);
  level.gravityComponents.reserve(64);
  level.renderComponents.reserve(2048);
  level.tileComponents.reserve(2048);
  level.transformComponents.reserve(2048);

  std::string currentValue;
  char c = '0';

  std::cout << "Creating scene" << "\n";

  int y = 0;
  int x = 0;
  int maxX = 0;

  createUI(0, 0, level);

  while (SDL_RWread(file, &c, 1, 1) == 1) {
    if (c == '\n') {
      y++;
      x = 0;
      continue;
    }
    if (c == ',') {
      if (currentValue.starts_with("PC")) {
        createPC(128 * x, 128 * y, level);
        createEmptyTile(128 * x, 128 * y, level);

      } else if (currentValue.starts_with("E")) {
        createEnemy(128 * x, 128 * y, level);
        createEmptyTile(128 * x, 128 * y, level);

      } else if (currentValue.starts_with("B")) {
        createBlock(currentValue, 128 * x, 128 * y, level);

      } else if (currentValue.starts_with("LS")) {
        createLadderStart(128 * x, 128 * y, level);

      } else if (currentValue.starts_with("L")) {
        createLadder(128 * x, 128 * y, level);

      } else if (currentValue == "") {
        createEmptyTile(128 * x, 128 * y, level);

      } else {
        std::cout << "Error: Scene creator: " << currentValue << " is unknown. Line " << y << " "
                  << x << std::endl;
      }

      x++;
      if (x > maxX) {
        maxX = x;
      }

      currentValue = "";
      continue;
    }

    else {
      currentValue += c;
    }
  }
  SDL_RWclose(file);

  level.levelLength = maxX * 128;
  level.levelHeight = y * 128;

  // Section 2.5: bake the static-collider tile grid for O(1) neighbour
  // queries in CollisionDetectionSystem. Cells without a static collider
  // remain TileGrid::EMPTY.
  level.staticTileGrid.resize(maxX, y + 1);
  for (int sid : level.staticCollisionIds) {
    if (TileComponent const *tile = level.tileComponents.get(sid)) {
      level.staticTileGrid.set(tile->tilePosition.x, tile->tilePosition.y, sid);
    }
  }

  return level;
}

void SceneCreator::createBlock(std::string const &gameObjectType, int x, int y, SceneInfo &level) {
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
  int id = level.getId();

  level.transformComponents.emplace(id, x, y, id);
  level.tileComponents.emplace(id, TILE::NONE, x / 128, y / 128);

  // render for testing
  SDL_Rect const sourceRect = {128, 384, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 1, id);

  level.ids++;
  level.tileRenderIds.emplace_back(id);
}

void SceneCreator::createBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = level.getId();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.transformComponents.emplace(id, x, y, id);
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 1, id);
  level.collisionComponents.emplace(id, destinationRect, id);
  level.tileComponents.emplace(id, TILE::SOLID, x / 128, y / 128);

  level.ids++;
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
}

void SceneCreator::createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = level.getId();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.transformComponents.emplace(id, x, y, id);
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 1, id);
  // NOTE: pre-existing behaviour — uses sourceRect (not destinationRect) for
  // collision. Preserved here to keep this refactor purely about storage.
  level.collisionComponents.emplace(id, sourceRect, id);
  level.tileComponents.emplace(id, TILE::SEMISOLID, x / 128, y / 128);

  level.ids++;
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
}

void SceneCreator::createPC(int x, int y, SceneInfo &level) {
  int id = 1;

  SDL_Rect const sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const rightAttackCollisionRect = {x + PLAYER_WIDTH, y, 64, PLAYER_HEIGHT};
  SDL_Rect const leftAttackCollisionRect = {x - PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2, 64,
                                            PLAYER_HEIGHT};

  level.transformComponents.emplace(id, x, y, id);
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 0, id);
  level.collisionComponents.emplace(id, collisionRect, id);
  level.gravityComponents.emplace(id, GRAVITY_STATE::AIRBORNE, id);
  AnimationComponent &animationComponent = level.animationComponents.emplace(id);
  animationComponent.parentID = id;
  level.controlComponents.emplace(id, CONTROL_STATE::AIRBORNE, id);
  level.healthComponents.emplace(id, 3);
  level.attackComponents.emplace(id, 36, 1, rightAttackCollisionRect, leftAttackCollisionRect);

  level.ids++;
  level.objectRenderIds.emplace_back(id);
}

void SceneCreator::createEnemy(int x, int y, SceneInfo &level) {
  int id = level.getId();

  SDL_Rect const sourceRect = {0, 512, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const rightAttackCollisionRect = {x + PLAYER_WIDTH, y, 64, PLAYER_HEIGHT};
  SDL_Rect const leftAttackCollisionRect = {x - PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2, 64,
                                            PLAYER_HEIGHT};

  TransformComponent &transformComponent = level.transformComponents.emplace(id, x, y, id);
  transformComponent.velocity.x = 4;
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 3, id);
  level.collisionComponents.emplace(id, collisionRect, id);
  level.gravityComponents.emplace(id, GRAVITY_STATE::AIRBORNE, id);
  level.healthComponents.emplace(id, 1);
  level.aiComponents.emplace(id);
  AnimationComponent &animationComponent = level.animationComponents.emplace(id);
  animationComponent.parentID = id;
  level.attackComponents.emplace(id, 36, 1, rightAttackCollisionRect, leftAttackCollisionRect);

  level.ids++;
  level.objectRenderIds.emplace_back(id);
}

void SceneCreator::createLadder(int x, int y, SceneInfo &level) {
  int id = level.getId();

  SDL_Rect const sourceRect = {0, 192, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, 128, 128};

  level.transformComponents.emplace(id, x, y, id);
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 1, id);
  level.collisionComponents.emplace(id, collisionRect, id);
  level.tileComponents.emplace(id, TILE::LADDER, x / 128, y / 128);

  level.ids++;
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
}

void SceneCreator::createLadderStart(int x, int y, SceneInfo &level) {
  int id = level.getId();

  SDL_Rect const sourceRect = {0, 192, 128, 64};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, 128, 64};

  level.transformComponents.emplace(id, x, y, id);
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 1, id);
  level.collisionComponents.emplace(id, collisionRect, id);
  level.tileComponents.emplace(id, TILE::LADDERSTART, x / 128, y / 128);

  level.ids++;
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
}

void SceneCreator::createUI(int x, int y, SceneInfo &level) {
  int id = 0;
  SDL_Rect const sourceRect = {0, 0, 31, 31};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.transformComponents.emplace(id, x, y, id);
  level.renderComponents.emplace(id, sourceRect, destinationRect, SDL_FLIP_NONE, 2, id);

  level.ids++;
  level.UIRenderIds.emplace_back(id);
}

json SceneCreator::getGameData() { return GameData::init(); }
