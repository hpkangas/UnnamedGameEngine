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

  // Bake the static-collider tile grid for O(1) neighbour queries.
  level.staticTileGrid.resize(maxX, y + 1);
  for (entt::entity sid : level.staticCollisionIds) {
    TileComponent const &tile = level.registry.get<TileComponent>(sid);
    level.staticTileGrid.set(tile.tilePosition.x, tile.tilePosition.y, sid);
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
  auto entity = level.createEntity();

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<TileComponent>(entity, TILE::NONE, x / 128, y / 128);

  SDL_Rect const sourceRect = {128, 384, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);

  level.tileRenderIds.emplace_back(entity);
}

void SceneCreator::createBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  auto entity = level.createEntity();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, destinationRect);
  level.registry.emplace<TileComponent>(entity, TILE::SOLID, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  auto entity = level.createEntity();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, sourceRect);
  level.registry.emplace<TileComponent>(entity, TILE::SEMISOLID, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createPC(int x, int y, SceneInfo &level) {
  auto entity = level.createEntity();
  level.playerEntity = entity;

  SDL_Rect const sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const rightAttackCollisionRect = {x + PLAYER_WIDTH, y, 64, PLAYER_HEIGHT};
  SDL_Rect const leftAttackCollisionRect = {x - PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2, 64,
                                            PLAYER_HEIGHT};

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 0);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<GravityComponent>(entity, GRAVITY_STATE::AIRBORNE);
  level.registry.emplace<AnimationComponent>(entity);
  level.registry.emplace<ControlComponent>(entity, CONTROL_STATE::AIRBORNE);
  level.registry.emplace<HealthComponent>(entity, 3);
  level.registry.emplace<AttackComponent>(entity, 36, 1, rightAttackCollisionRect,
                                          leftAttackCollisionRect);

  level.objectRenderIds.emplace_back(entity);
}

void SceneCreator::createEnemy(int x, int y, SceneInfo &level) {
  auto entity = level.createEntity();

  SDL_Rect const sourceRect = {0, 512, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect const rightAttackCollisionRect = {x + PLAYER_WIDTH, y, 64, PLAYER_HEIGHT};
  SDL_Rect const leftAttackCollisionRect = {x - PLAYER_WIDTH / 2, y - PLAYER_HEIGHT / 2, 64,
                                            PLAYER_HEIGHT};

  level.registry.emplace<TransformComponent>(entity, x, y);
  auto &transformComponent = level.registry.get<TransformComponent>(entity);
  transformComponent.velocity.x = 4;
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 3);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<GravityComponent>(entity, GRAVITY_STATE::AIRBORNE);
  level.registry.emplace<HealthComponent>(entity, 1);
  level.registry.emplace<AIComponent>(entity);
  level.registry.emplace<AnimationComponent>(entity);
  level.registry.emplace<AttackComponent>(entity, 36, 1, rightAttackCollisionRect,
                                          leftAttackCollisionRect);

  level.objectRenderIds.emplace_back(entity);
}

void SceneCreator::createLadder(int x, int y, SceneInfo &level) {
  auto entity = level.createEntity();

  SDL_Rect const sourceRect = {0, 192, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, 128, 128};

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<TileComponent>(entity, TILE::LADDER, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createLadderStart(int x, int y, SceneInfo &level) {
  auto entity = level.createEntity();

  SDL_Rect const sourceRect = {0, 192, 128, 64};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, 128, 64};

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<TileComponent>(entity, TILE::LADDERSTART, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createUI(int x, int y, SceneInfo &level) {
  auto entity = level.createEntity();
  SDL_Rect const sourceRect = {0, 0, 31, 31};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.registry.emplace<TransformComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 2);

  level.UIRenderIds.emplace_back(entity);
}

json SceneCreator::getGameData() { return GameData::init(); }
