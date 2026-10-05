#include "scenecreator.hpp"
#include "../components/tilecomponent.hpp"
#include "entitycreator.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>



// --- End: JSON template helpers and template-driven entity creation ---

SceneInfo SceneCreator::createLevel(SDL_RWops *file) {
  SceneInfo level;

  std::string currentValue;
  char c = '0';

  std::cout << "Creating scene" << "\n";

  int y = 0;
  int x = 0;
  int maxX = 0;

  while (SDL_RWread(file, &c, 1, 1) == 1) {
    if (c == '\n') {
      y++;
      x = 0;
      continue;
    }
    if (c == ',') {
      if (currentValue.starts_with("PC")) {
        EntityCreator::createPC(Position{x, y}, level);
        EntityCreator::createEmptyTile(Position{x, y}, level);

      } else if (currentValue.starts_with("E")) {
        EntityCreator::createEnemy(Position{x, y}, level);
        EntityCreator::createEmptyTile(Position{x, y}, level);

      } else if (currentValue.starts_with("B")) {
        EntityCreator::createBlock(currentValue, Position{x, y}, level);

      // } else if (currentValue.starts_with("LS")) {
      //   EntityCreator::createLadderStart(Position{x, y}, level);

      // } else if (currentValue.starts_with("L")) {
      //   EntityCreator::createLadder(Position{x, y}, level);

      } else if (currentValue == "") {
        EntityCreator::createEmptyTile(Position{x, y}, level);

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
    } else {
      currentValue += c;
    }
  }
  SDL_RWclose(file);

  level.levelLength = maxX * 128;
  level.levelHeight = y * 128;

  // Bake the static-collider tile grid for O(1) neighbour queries.
  level.staticTileGrid.resize(maxX, y + 1);
  auto tileView = level.registry.view<TileComponent>();
  for (entt::entity tileEntity : tileView) {
    TileComponent const &tile = level.registry.get<TileComponent>(tileEntity);
    level.staticTileGrid.set(tile.tilePosition.x, tile.tilePosition.y, tileEntity);
  }

  return level;
}




