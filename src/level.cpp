#include "level.hpp"
#include "SDL_render.h"
#include "components/animationcomponent.hpp"
#include "components/collisioncomponent.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
#include "entities/entity.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_rwops.h>
#include <asm-generic/errno.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

LevelInfo Level::createLevel(SDL_RWops *Handle) {
  LevelInfo level;
  std::string currentValue;
  char c;
  std::vector<std::string> values;
  std::cout << "creating level" << "\n";
  loadTextures();
  while (SDL_RWread(Handle, &c, 1, 1) == 1) {

    if (c == '\n') {
      continue;
    }

    if (c == ',') {

      if (currentValue != "|") {

        values.emplace_back(currentValue);

      } else {
        std::string gameObjectType = values.at(0);
        int x = std::stoi(values.at(1));
        int y = std::stoi(values.at(2));

        if (gameObjectType.substr(0, 2) == "PC") {
          createPC(x, y, level);
        }

        if (gameObjectType.substr(0, 1) == "B") {
          createBlock(gameObjectType, x, y, level);
        }

        values.clear();
      }
      currentValue = "";
      continue;
    }

    currentValue += c;
  }

  return level;
}

void Level::createBlock(std::string gameObjectType, int x, int y, LevelInfo &level) {
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

void Level::createBlock1(int x, int y, LevelInfo &level) { createBlockEntity(x, y, 0, level); }
void Level::createBlock2(int x, int y, LevelInfo &level) { createBlockEntity(x, y, 128, level); }
void Level::createBlock3(int x, int y, LevelInfo &level) { createBlockEntity(x, y, 256, level); }

void Level::createBlockEntity(int x, int y, int sourceX, LevelInfo &level) {
  int id = getID();
  std::cout << id << "\n";

  std::unique_ptr<Entity> entity = std::make_unique<Entity>();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();

  transformComponent->velocity = {0, 0};
  transformComponent->position = {x, y};
  transformComponent->parentID = id;

  renderComponent->sourceRect = {sourceX, 0, 128, 64};
  renderComponent->destinationRect.h = renderComponent->sourceRect.h;
  renderComponent->destinationRect.w = renderComponent->sourceRect.w;
  renderComponent->destinationRect.x = transformComponent->position.x;
  renderComponent->destinationRect.y = transformComponent->position.y;
  renderComponent->parentID = id;

  collisionComponent->collisionRect.h = renderComponent->destinationRect.h;
  collisionComponent->collisionRect.w = renderComponent->destinationRect.w;
  collisionComponent->collisionRect.x = transformComponent->position.x;
  collisionComponent->collisionRect.y = transformComponent->position.y + 4;
  collisionComponent->parentID = id;

  // TODO Should probably be some constant value loaded at the start
  renderComponent->objTexture = textures.at(1);
  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.ids++;
}

void Level::loadTextures() {
  std::cout << "Loading textures...\n";
  textures.emplace_back(TextureManager::LoadTexture("assets/baleog.png"));
  textures.emplace_back(TextureManager::LoadTexture("assets/tiles.png"));
}

void Level::destroyTextures() {
  for (auto &tex : textures) {
    SDL_DestroyTexture(tex);
  }
}

void Level::createPC(int x, int y, LevelInfo &level) {
  int id = getID();
  std::cout << id << "\n";

  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();
  auto gravityComponent = std::make_unique<GravityComponent>();
  auto animationComponent = std::make_unique<AnimationComponent>();

  transformComponent->velocity = {0, 0};
  transformComponent->position = {x, y};
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
  collisionComponent->collisionRect = {transformComponent->position.x + 28,
                                       transformComponent->position.y, PLAYER_WIDTH - 56,
                                       PLAYER_HEIGHT};

  collisionComponent->parentID = id;

  gravityComponent->gravityState = INAIR;
  gravityComponent->parentID = id;
  animationComponent->parentID = id;

  level.tcs.emplace(id, (std::move(transformComponent)));
  level.rcs.emplace(id, (std::move(renderComponent)));
  level.ccs.emplace(id, (std::move(collisionComponent)));
  level.gcs.emplace(id, (std::move(gravityComponent)));
  level.acs.emplace(id, (std::move(animationComponent)));
  level.ids++;
}

int Level::getID() {
  static int id = -1;
  id++;
  return id;
}
