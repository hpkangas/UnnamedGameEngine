#include "scenecreator.hpp"
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
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

SceneInfo SceneCreator::createLevel(SDL_RWops *Handle) {
  SceneInfo level;
  std::string currentValue;
  char c;
  std::vector<std::string> values;

  std::cout << "creating scene" << "\n";
  loadTextures();

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {
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

  return level;
}

void SceneCreator::createBlock(std::string gameObjectType, int x, int y, SceneInfo &level) {
  if (gameObjectType == "B1") {
    createBlock1(x, y, level);
  }
  if (gameObjectType == "B2") {
    createBlock2(x, y, level);
  }
  if (gameObjectType == "B3") {
    createBlock3(x, y, level);
  }
  createBlock1(0, 0, level);
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

void SceneCreator::createBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = getID();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();
  auto tileComponent = std::make_unique<TileComponent>();

  tileComponent->tile = TILE::SOLID;

  transformComponent->setVelocity({0, 0});
  transformComponent->setPosition({x, y});
  transformComponent->parentID = id;

  renderComponent->sourceRect = {sourceX, 0, 128, 64};
  renderComponent->destinationRect.h = renderComponent->sourceRect.h;
  renderComponent->destinationRect.w = renderComponent->sourceRect.w;
  renderComponent->destinationRect.x = transformComponent->getXpos();
  renderComponent->destinationRect.y = transformComponent->getYpos();
  renderComponent->parentID = id;

  collisionComponent->collisionRect.h = renderComponent->destinationRect.h;
  collisionComponent->collisionRect.w = renderComponent->destinationRect.w;
  collisionComponent->collisionRect.x = transformComponent->getXpos();
  collisionComponent->collisionRect.y = transformComponent->getYpos();
  collisionComponent->parentID = id;

  // TODO Should probably be some constant value loaded at the start
  renderComponent->objTexture = textures.at(1);
  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
}

void SceneCreator::createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  int id = getID();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();
  auto tileComponent = std::make_unique<TileComponent>();

  tileComponent->tile = TILE::SEMISOLID;

  transformComponent->setVelocity({0, 0});
  transformComponent->setPosition({x, y});
  transformComponent->parentID = id;

  renderComponent->sourceRect = {sourceX, 0, 128, 64};
  renderComponent->destinationRect.h = renderComponent->sourceRect.h;
  renderComponent->destinationRect.w = renderComponent->sourceRect.w;
  renderComponent->destinationRect.x = transformComponent->getXpos();
  renderComponent->destinationRect.y = transformComponent->getYpos();
  renderComponent->parentID = id;

  collisionComponent->collisionRect.h = renderComponent->destinationRect.h;
  collisionComponent->collisionRect.w = renderComponent->destinationRect.w;
  collisionComponent->collisionRect.x = transformComponent->getXpos();
  collisionComponent->collisionRect.y = transformComponent->getYpos() + 4;
  collisionComponent->parentID = id;

  // TODO Should probably be some constant value loaded at the start
  renderComponent->objTexture = textures.at(1);
  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
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
  int id = getID();

  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();
  auto gravityComponent = std::make_unique<GravityComponent>();
  auto animationComponent = std::make_unique<AnimationComponent>();
  auto controlComponent = std::make_unique<ControlComponent>();

  transformComponent->setVelocity({0, 0});
  transformComponent->setPosition({x, y});
  transformComponent->parentID = id;

  renderComponent->sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
  // TODO Should probably be some constant value loaded at the start
  renderComponent->objTexture = textures.at(0);
  renderComponent->flip = SDL_FLIP_NONE;
  renderComponent->destinationRect = {x, y, renderComponent->sourceRect.w,
                                      renderComponent->sourceRect.h};
  renderComponent->parentID = id;

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  collisionComponent->collisionRect = {transformComponent->getXpos() + 28,
                                       transformComponent->getYpos(), PLAYER_WIDTH - 56,
                                       PLAYER_HEIGHT};
  collisionComponent->parentID = id;

  controlComponent->setControlState(CONTROL_STATE::AIRBORNE);
  controlComponent->parentID = id;

  gravityComponent->gravityState = GRAVITY_STATE::AIRBORNE;
  gravityComponent->parentID = id;

  animationComponent->parentID = id;

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.gcs.emplace(id, (std::move(gravityComponent)));
  level.acs.emplace(id, (std::move(animationComponent)));
  level.controlComponents.emplace(id, (std::move(controlComponent)));
  level.ids++;
}

void SceneCreator::createLadder(int x, int y, SceneInfo &level) {
  int id = getID();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto tileComponent = std::make_unique<TileComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();

  transformComponent->setPosition({x, y});
  transformComponent->parentID = id;

  renderComponent->sourceRect = {0, 192, 128, 128};
  renderComponent->destinationRect = {x, y, renderComponent->sourceRect.w,
                                      renderComponent->sourceRect.h};
  renderComponent->objTexture = textures.at(1);
  renderComponent->parentID = id;

  collisionComponent->collisionRect = {x, y, 128, 128};
  collisionComponent->parentID = id;

  tileComponent->tile = TILE::LADDER;
  tileComponent->parentID = id;

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
}

void SceneCreator::createLadderStart(int x, int y, SceneInfo &level) {
  int id = getID();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto tileComponent = std::make_unique<TileComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();

  transformComponent->setPosition({x, y});
  transformComponent->parentID = id;

  renderComponent->sourceRect = {0, 128, 128, 64};
  renderComponent->destinationRect = {x, y, renderComponent->sourceRect.w,
                                      renderComponent->sourceRect.h};
  renderComponent->objTexture = textures.at(1);
  renderComponent->parentID = id;

  collisionComponent->collisionRect = {x, y, 128, 64};
  collisionComponent->parentID = id;

  tileComponent->tile = TILE::LADDERSTART;
  tileComponent->parentID = id;

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.tiles.emplace(id, (std::move(tileComponent)));
  level.ids++;
}

int SceneCreator::getID() {
  static int id = -1;
  id++;
  return id;
}
