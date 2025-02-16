#include "level.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
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
          // Entity *entity = createPCEntity(x, y);
          std::cout << "create pc" << "\n";

          level.entities2.emplace_back(createPCEntity(x, y));
          // level.renderables.emplace_back(&entity->renderComponent);
          // Player *player = createPlayerCharacter(gameObjectType, x, y);
          // level.playercharacters.emplace_back(player);
          // level.renderables.emplace_back(&player->renderComponent);
        }

        if (gameObjectType.substr(0, 1) == "B") {
          level.entities2.emplace_back(createBlock(gameObjectType, x, y));
        }

        if (gameObjectType.substr(0, 1) == "E") {
          //  Entity *entity = createBlockEntity(x, y);

          //  level.entities.emplace_back(entity);
          std::cout << "create block" << "\n";
          level.entities2.emplace_back(createBlockEntity(x, y, 0));
          // level.renderables.emplace_back(&entity->renderComponent);
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

std::unique_ptr<Entity> Level::createBlock(std::string gameObjectType, int x, int y) {
  if (gameObjectType == "B1") {
    return createBlock1(x, y);
  }
  if (gameObjectType == "B2") {
    return createBlock2(x, y);
  }
  if (gameObjectType == "B3") {
    return createBlock3(x, y);
  }
  return createBlock1(0, 0);
}

std::unique_ptr<Entity> Level::createBlock1(int x, int y) { return createBlockEntity(x, y, 0); }
std::unique_ptr<Entity> Level::createBlock2(int x, int y) { return createBlockEntity(x, y, 128); }
std::unique_ptr<Entity> Level::createBlock3(int x, int y) { return createBlockEntity(x, y, 256); }

std::unique_ptr<Entity> Level::createBlockEntity(int x, int y, int sourceX) {
  std::unique_ptr<Entity> entity = std::make_unique<Entity>();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();

  transformComponent->velocity = {0, 0};
  transformComponent->position = {x, y};
  transformComponent->parentID = entity->ID;

  renderComponent->sourceRect = {sourceX, 0, 128, 64};
  renderComponent->destinationRect.h = renderComponent->sourceRect.h;
  renderComponent->destinationRect.w = renderComponent->sourceRect.w;
  renderComponent->destinationRect.x = transformComponent->position.x;
  renderComponent->destinationRect.y = transformComponent->position.y;
  renderComponent->parentID = entity->ID;

  collisionComponent->collisionRect.h = renderComponent->destinationRect.h;
  collisionComponent->collisionRect.w = renderComponent->destinationRect.w;
  collisionComponent->collisionRect.x = transformComponent->position.x;
  collisionComponent->collisionRect.y = transformComponent->position.y + 4;
  collisionComponent->parentID = entity->ID;

  renderComponent->objTexture = TextureManager::LoadTexture("assets/tiles.png");
  entity->add(std::move(transformComponent));
  entity->add(std::move(renderComponent));
  entity->add(std::move(collisionComponent));
  std::cout << entity->ID << "\n";

  return entity;
}

std::unique_ptr<Entity> Level::createPCEntity(int x, int y) {
  std::unique_ptr<Entity> entity = std::make_unique<Entity>();
  auto transformComponent = std::make_unique<TransformComponent>();
  auto renderComponent = std::make_unique<RenderComponent>();
  auto collisionComponent = std::make_unique<CollisionComponent>();
  auto gravityComponent = std::make_unique<GravityComponent>();
  auto animationComponent = std::make_unique<AnimationComponent>();

  transformComponent->velocity = {0, 0};
  transformComponent->position = {x, y};
  transformComponent->parentID = entity->ID;

  renderComponent->sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};

  renderComponent->objTexture = TextureManager::LoadTexture("assets/baleog.png");

  renderComponent->flip = SDL_FLIP_NONE;

  renderComponent->destinationRect = {x, y, renderComponent->sourceRect.w,
                                      renderComponent->sourceRect.h};

  renderComponent->parentID = entity->ID;

  // TODO sprite offsets-> should be defined inside the specific character
  // classes (once decided on)
  collisionComponent->collisionRect = {transformComponent->position.x + 28,
                                       transformComponent->position.y, PLAYER_WIDTH - 56,
                                       PLAYER_HEIGHT};

  collisionComponent->parentID = entity->ID;

  gravityComponent->gravityState = INAIR;
  gravityComponent->parentID = entity->ID;
  animationComponent->parentID = entity->ID;

  entity->add(std::move(transformComponent));
  entity->add(std::move(renderComponent));
  entity->add(std::move(collisionComponent));
  entity->add(std::move(gravityComponent));
  entity->add(std::move(animationComponent));
  std::cout << entity->ID << "\n";

  return entity;
}
