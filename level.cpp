#include "level.hpp"
#include "components/rendercomponent.hpp"
#include "components/transformcomponent.hpp"
#include "entities/block.hpp"
#include "entities/player.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <string>
#include <vector>

LevelInfo Level::createLevel(SDL_RWops *Handle) {
  LevelInfo level;
  std::string currentValue;
  char c;
  std::vector<std::string> values;

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
          Player *player = createPlayerCharacter(gameObjectType, x, y);
          level.playercharacters.emplace_back(player);
          level.renderables.emplace_back(&player->renderComponent);
        }

        if (gameObjectType.substr(0, 1) == "B") {
          Block *block = createBlock(gameObjectType, x, y);
          level.blocks.emplace_back(block);
          level.renderables.emplace_back(&block->renderComponent);
        }

        if (gameObjectType.substr(0, 1) == "E") {
          Entity *entity = createBlockEntity(x, y);
          level.entities.emplace_back(entity);
          level.renderables.emplace_back(static_cast<RenderComponent*>(&entity->components[1]));

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

Entity *Level::createBlockEntity(int x, int y){
  Entity *entity = new Entity();
  TransformComponent transformComponent;
  transformComponent.position = {x,y};
  transformComponent.velocity = {0,0};

  RenderComponent renderComponent;
  renderComponent.sourceRect = {0,0,128,64};
  renderComponent.destinationRect.h = renderComponent.sourceRect.h;
  renderComponent.destinationRect.w = renderComponent.sourceRect.w;
  renderComponent.destinationRect.x = transformComponent.position.x;
  renderComponent.destinationRect.y = transformComponent.position.y;

  CollisionComponent collisionComponent;
  collisionComponent.collisionRect.h = renderComponent.destinationRect.h;
  collisionComponent.collisionRect.w = renderComponent.destinationRect.w;
  collisionComponent.collisionRect.x = transformComponent.position.x;
  collisionComponent.collisionRect.y = transformComponent.position.y + 4;

  
  renderComponent.objTexture = TextureManager::LoadTexture("assets/tiles.png");

  entity->add(transformComponent);
  entity->add(renderComponent);
  entity->add(collisionComponent);

  return entity;
}
