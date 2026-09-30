#include "scenecreator.hpp"
#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/shootcomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../include/json/json.hpp"
#include "../texturemanager.hpp"
#include "../utilities/constants.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "entitycreator.hpp"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>



// --- End: JSON template helpers and template-driven entity creation ---

SceneInfo SceneCreator::createLevel(SDL_RWops *file) {
  SceneInfo level;
  EntityCreator entityCreator;

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
        entityCreator.createPC(128 * x, 128 * y, level);
        entityCreator.createEmptyTile(128 * x, 128 * y, level);

      } else if (currentValue.starts_with("E")) {
        entityCreator.createEnemy(128 * x, 128 * y, level);
        entityCreator.createEmptyTile(128 * x, 128 * y, level);

      } else if (currentValue.starts_with("B")) {
        entityCreator.createBlock(currentValue, 128 * x, 128 * y, level);

      } else if (currentValue.starts_with("LS")) {
        entityCreator.createLadderStart(128 * x, 128 * y, level);

      } else if (currentValue.starts_with("L")) {
        entityCreator.createLadder(128 * x, 128 * y, level);

      } else if (currentValue == "") {
        entityCreator.createEmptyTile(128 * x, 128 * y, level);

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
  for (entt::entity sid : level.staticCollisionIds) {
    TileComponent const &tile = level.registry.get<TileComponent>(sid);
    level.staticTileGrid.set(tile.tilePosition.x, tile.tilePosition.y, sid);
  }

  return level;
}




