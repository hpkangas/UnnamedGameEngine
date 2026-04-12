#include "scenecreator.hpp"
#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../gamedata.hpp"
#include "../texturemanager.hpp"
#include "../utilities/constants.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include <SDL2/SDL_rwops.h>
#include <asm-generic/errno.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

// json SceneCreator::data{getGameData()};
using namespace Constants;
SceneInfo SceneCreator::createLevel(SDL_RWops *file) {
  SceneInfo level;
  std::string currentValue;
  char c = '0';

  std::cout << "Creating scene" << "\n";
  loadTextures();

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
        std::cout << "Error: Scene creator: " << currentValue << "is unknown. Line " << y << " "
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
  int id = getId();

  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  transformComponent->parentID = id;
  auto tileComponent = std::make_unique<TileComponent>(TILE::NONE, x / 128, y / 128);

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.tileComponents.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.tileIds.emplace_back(id);

  // render for testing
  SDL_Rect sourceRect = {128, 384, 128, 128};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.tileRenderIds.emplace_back(id);
}

void SceneCreator::createBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto collisionComponent = std::make_unique<CollisionComponent>(destinationRect, id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::SOLID, x / 128, y / 128);

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.collisionComponents.emplace(id, (std::move(collisionComponent)));
  level.tileComponents.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
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
  auto tileComponent = std::make_unique<TileComponent>(TILE::SEMISOLID, x / 128, y / 128);

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.collisionComponents.emplace(id, (std::move(collisionComponent)));
  level.tileComponents.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::loadTextures() {
  std::cout << "Loading textures...\n";
  textures.emplace_back(TextureManager::LoadTexture("assets/baleog.png"));
  textures.emplace_back(TextureManager::LoadTexture("assets/tiles.png"));
  textures.emplace_back(TextureManager::LoadTexture("assets/UI.png"));
}

void SceneCreator::destroyTextures() const {
  for (auto &tex : textures) {
    SDL_DestroyTexture(tex);
  }
}

void SceneCreator::createPC(int x, int y, SceneInfo &level) {
  int id = 1;

  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(0), id);
  SDL_Rect collisionRect = {transformComponent->position.x, transformComponent->position.y,
                            PLAYER_WIDTH, PLAYER_HEIGHT};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);
  auto gravityComponent = std::make_unique<GravityComponent>(GRAVITY_STATE::AIRBORNE, id);
  auto animationComponent = std::make_unique<AnimationComponent>();
  auto controlComponent = std::make_unique<ControlComponent>(CONTROL_STATE::AIRBORNE, id);
  auto healthComponent = std::make_unique<HealthComponent>(3);
  auto attackComponent = std::make_unique<AttackComponent>(36, 1);

  animationComponent->parentID = id;

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.collisionComponents.emplace(id, (std::move(collisionComponent)));
  level.gravityComponents.emplace(id, (std::move(gravityComponent)));
  level.animationComponents.emplace(id, (std::move(animationComponent)));
  level.controlComponents.emplace(id, (std::move(controlComponent)));
  level.healthComponents.emplace(id, (std::move(healthComponent)));
  level.attackComponents.emplace(id, (std::move(attackComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.objectRenderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.gravityIds.emplace_back(id);
  level.controlIds.emplace_back(id);
  level.animationIds.emplace_back(id);
  level.healthIds.emplace_back(id);
  level.attackIds.emplace_back(id);
}

void SceneCreator::createEnemy(int x, int y, SceneInfo &level) {
  int id = getId();

  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  transformComponent->velocity.x = 4;
  SDL_Rect sourceRect = {0, 512, PLAYER_WIDTH, PLAYER_HEIGHT};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(0), id);
  SDL_Rect collisionRect = {transformComponent->position.x + 4, transformComponent->position.y,
                            PLAYER_WIDTH - 4, PLAYER_HEIGHT};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);
  auto gravityComponent = std::make_unique<GravityComponent>(GRAVITY_STATE::AIRBORNE, id);
  auto healthComponent = std::make_unique<HealthComponent>(3);
  auto aiComponent = std::make_unique<AIComponent>();

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.collisionComponents.emplace(id, (std::move(collisionComponent)));
  level.gravityComponents.emplace(id, (std::move(gravityComponent)));
  level.healthComponents.emplace(id, (std::move(healthComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.objectRenderIds.emplace_back(id);
  level.collisionIds.emplace_back(id);
  level.gravityIds.emplace_back(id);
  level.healthIds.emplace_back(id);
  level.AIIds.emplace_back(id);
}

void SceneCreator::createLadder(int x, int y, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 192, 128, 128};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::LADDER, x / 128, y / 128);
  SDL_Rect collisionRect = {x, y, 128, 128};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.collisionComponents.emplace(id, (std::move(collisionComponent)));
  level.tileComponents.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::createLadderStart(int x, int y, SceneInfo &level) {
  int id = getId();
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 192, 128, 64};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(1), id);
  auto tileComponent = std::make_unique<TileComponent>(TILE::LADDERSTART, x / 128, y / 128);
  SDL_Rect collisionRect = {x, y, 128, 64};
  auto collisionComponent = std::make_unique<CollisionComponent>(collisionRect, id);

  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.collisionComponents.emplace(id, (std::move(collisionComponent)));
  level.tileComponents.emplace(id, (std::move(tileComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.tileRenderIds.emplace_back(id);
  level.staticCollisionIds.emplace_back(id);
  level.tileIds.emplace_back(id);
}

void SceneCreator::createUI(int x, int y, SceneInfo &level) {
  int id = 0;
  auto transformComponent = std::make_unique<TransformComponent>(x, y, id);
  SDL_Rect sourceRect = {0, 0, 31, 31};
  SDL_Rect destinationRect = {x, y, sourceRect.w, sourceRect.h};
  auto renderComponent = std::make_unique<RenderComponent>(sourceRect, destinationRect,
                                                           SDL_FLIP_NONE, textures.at(2), id);
  level.transformComponents.emplace(id, (std::move(transformComponent)));
  level.renderComponents.emplace(id, (std::move(renderComponent)));
  level.ids++;
  level.transformIds.emplace_back(id);
  level.UIRenderIds.emplace_back(id);
}

int SceneCreator::getId() {
  static int id = 2;
  id++;
  return id;
}

json SceneCreator::getGameData() { return GameData::init(); }
