#include "level.hpp"
#include "components/component.hpp"
#include "components/gravitycomponent.hpp"
#include "components/rendercomponent.hpp"
#include "components/transformcomponent.hpp"
#include "constants.hpp"
#include "entities/block.hpp"
#include "entities/player.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_rwops.h>
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
          Entity *entity = createPCEntity(x, y);
          level.entities.emplace_back(entity);
          level.renderables.emplace_back(&entity->renderComponent);
          // Player *player = createPlayerCharacter(gameObjectType, x, y);
          // level.playercharacters.emplace_back(player);
          // level.renderables.emplace_back(&player->renderComponent);
        }

        if (gameObjectType.substr(0, 1) == "B") {
          Block *block = createBlock(gameObjectType, x, y);
          level.blocks.emplace_back(block);
          level.renderables.emplace_back(&block->renderComponent);
        }

        if (gameObjectType.substr(0, 1) == "E") {
          //  Entity *entity = createBlockEntity(x, y);

          //  level.entities.emplace_back(entity);
          std::cout << "entity creation" << "\n";
          level.entitieswip.emplace(getID(), createBlockEntity2(x, y));
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

Player *Level::createPlayerCharacter(std::string gameObjectType, int x, int y) {
  // some general creator function here too rather than calling constructor straight away?
  // Player player;

  if (gameObjectType == "PC1") {
    return new Player(x, y, "Erik");
  }
  if (gameObjectType == "PC2") {
    return new Player(x, y, "Olaf");
  }
  if (gameObjectType == "PC3") {
    return new Player(x, y, "Baleog");
  }
  return new Player(0, 0, "NOT REAL");
}
Block *Level::createBlock1(int x, int y) { return new Block(x, y, 0); }
Block *Level::createBlock2(int x, int y) { return new Block(x, y, 128); }
Block *Level::createBlock3(int x, int y) { return new Block(x, y, 256); }

Block *Level::createBlock(std::string gameObjectType, int x, int y) {
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

ComponentVector Level::createBlockEntity2(int x, int y) {
  ComponentVector components;
  auto tc = std::make_unique<TransformComponent>();
  auto rc = std::make_unique<RenderComponent>();
  auto cc = std::make_unique<CollisionComponent>();

  tc->velocity = {2, 0};
  tc->position = {x, y};
  rc->sourceRect = {0, 0, 128, 64};
  rc->destinationRect.h = rc->sourceRect.h;
  rc->destinationRect.w = rc->sourceRect.w;
  rc->destinationRect.x = tc->position.x;
  rc->destinationRect.y = tc->position.y;
  cc->collisionRect.h = rc->destinationRect.h;
  cc->collisionRect.w = rc->destinationRect.w;
  cc->collisionRect.x = tc->position.x;
  cc->collisionRect.y = tc->position.y;
  rc->objTexture = TextureManager::LoadTexture("assets/tiles.png");

  components.emplace_back(std::move(tc));
  components.emplace_back(std::move(rc));
  components.emplace_back(std::move(cc));

  return components;
}

std::vector<IComponent *> Level::createPCEntity2(int x, int y) {
  Entity *entity = new Entity();
  TransformComponent tc;
  RenderComponent rc;
  CollisionComponent cc;
  GravityComponent gc;
  std::vector<IComponent *> components;
  tc.velocity = {2, 0};
  tc.position = {x, y};
  rc.sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
  rc.objTexture = TextureManager::LoadTexture("assets/baleog.png");
  rc.flip = SDL_FLIP_NONE;
  rc.destinationRect = {x, y, rc.sourceRect.w, rc.sourceRect.h};

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  cc.collisionRect = {tc.position.x + 28, tc.position.y, PLAYER_WIDTH - 56, PLAYER_HEIGHT};

  gc.gravityState = GROUNDED;

  entity->add(std::make_unique<TransformComponent>(tc));
  entity->add(std::make_unique<RenderComponent>(rc));
  entity->add(std::make_unique<CollisionComponent>(cc));
  entity->add(std::make_unique<GravityComponent>(gc));

  return components;
}

Entity *Level::createBlockEntity(int x, int y) {
  Entity *entity = new Entity();
  entity->transformComponent.velocity = {2, 0};
  entity->transformComponent.position = {x, y};

  entity->renderComponent.sourceRect = {0, 0, 128, 64};
  entity->renderComponent.destinationRect.h = entity->renderComponent.sourceRect.h;
  entity->renderComponent.destinationRect.w = entity->renderComponent.sourceRect.w;
  entity->renderComponent.destinationRect.x = entity->transformComponent.position.x;
  entity->renderComponent.destinationRect.y = entity->transformComponent.position.y;

  entity->collisionComponent.collisionRect.h = entity->renderComponent.destinationRect.h;
  entity->collisionComponent.collisionRect.w = entity->renderComponent.destinationRect.w;
  entity->collisionComponent.collisionRect.x = entity->transformComponent.position.x;
  entity->collisionComponent.collisionRect.y = entity->transformComponent.position.y + 4;

  entity->renderComponent.objTexture = TextureManager::LoadTexture("assets/tiles.png");

  return entity;
}

Entity *Level::createPCEntity(int x, int y) {
  Entity *entity = new Entity();
  entity->transformComponent.velocity = {2, 0};
  entity->transformComponent.position = {x, y};

  entity->renderComponent.sourceRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};

  entity->renderComponent.objTexture = TextureManager::LoadTexture("assets/baleog.png");

  entity->renderComponent.flip = SDL_FLIP_NONE;

  entity->renderComponent.destinationRect = {x, y, entity->renderComponent.sourceRect.w,
                                             entity->renderComponent.sourceRect.h};

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  entity->collisionComponent.collisionRect = {entity->transformComponent.position.x + 28,
                                              entity->transformComponent.position.y,
                                              PLAYER_WIDTH - 56, PLAYER_HEIGHT};

  entity->gravityComponent.gravityState = GROUNDED;

  return entity;
}

int Level::getID() {
  static int id = 0;
  id++;
  return id;
}