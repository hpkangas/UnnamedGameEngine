#include "scenecreator.hpp"
#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/shootcomponent.hpp"
#include "../components/tilecomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../include/json/json.hpp"
#include "../texturemanager.hpp"
#include "../utilities/constants.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include <SDL2/SDL_rwops.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using namespace Constants;
// --- Begin: JSON template helpers and template-driven entity creation ---

struct SpriteDef {
  std::string texture;
  SDL_Rect sourceRect;
  int layer;
};

static SpriteDef parseSpriteFromJSON(const nlohmann::json &entityDef) {
  SpriteDef def;
  if (entityDef.contains("sprite")) {
    auto &s = entityDef["sprite"];
    def.texture = s.value("texture", "error.png");

    auto &r = s["sourceRect"];
    def.sourceRect = {r.value("x", 0), r.value("y", 0), r.value("w", 128), r.value("h", 128)};

    def.layer = s["layer"];
  }
  return def;
}
// cached templates from data/data.json
static nlohmann::json g_entityTemplates;
static bool loadEntityTemplatesOnce() {
  if (!g_entityTemplates.is_null()) {
    return true;
  }

  std::string dataFilePath = "data/data.json";
  std::ifstream ifs(dataFilePath);
  if (!ifs.is_open()) {
    std::cout << "SceneCreator: failed to open " << dataFilePath << "\n";
    return false;
  }
  try {
    ifs >> g_entityTemplates;
  } catch (const std::exception &e) {
    std::cout << "SceneCreator: json parse error: " << e.what() << std::endl;
    g_entityTemplates = nlohmann::json(); // empty
    return false;
  }
  return true;
}

static nlohmann::json findTemplateByType(const std::string_view &type,
                                         const std::string_view &variant = "") {
  if (!loadEntityTemplatesOnce()) {
    return nlohmann::json();
  }
  if (!g_entityTemplates.contains("entities")) {
    return nlohmann::json();
  }
  for (auto const &entDef : g_entityTemplates["entities"]) {
    if (entDef.value("type", "") == type) {
      if (variant.empty()) {
        return entDef;
      }
      if (entDef.value("variant", "") == variant) {
        return entDef;
      }
    }
  }
  return nlohmann::json();
}

// Map texture filename (from the JSON) to TextureManager index.
// The game loads textures in a fixed order in TextureManager::loadTextures().
// Adjust mapping if you change load order or add more textures.
static int textureIndexFromName(const std::string &textureName) {
  return TextureManager::getTextureIndex(textureName);
}

static void createEntityFromTemplate(const nlohmann::json &def, int x, int y, SceneInfo &level) {
  if (def.is_null()) {
    return;
  }

  auto entity = level.registry.create();

  // Transform
  level.registry.emplace<PositionComponent>(entity, x, y);
  if (def.contains("velocity")) {
    // Optionally set velocity if present
    level.registry.emplace<VelocityComponent>(entity, static_cast<float>(def["velocity"].value("x", 0.0)),
                                             static_cast<float>(def["velocity"].value("y", 0.0)));
  }

  // Sprite/render (if present)
  SpriteDef sprite = parseSpriteFromJSON(def);
  if (!sprite.texture.empty()) {
    int texIndex = textureIndexFromName(sprite.texture);
    SDL_Rect src = sprite.sourceRect;
    SDL_Rect dst = {x, y, src.w, src.h};
    level.registry.emplace<RenderComponent>(entity, src, dst, SDL_FLIP_NONE, texIndex);
    if (def.contains("animation")) {
      auto &ac = level.registry.emplace<AnimationComponent>(entity);
      auto animationKey = def["animation"];

      if (animationKey.contains("WALK")) {
        ac.walking.spriteSheetY = animationKey["WALK"]["spriteSheetY"];
        ac.walking.maxFrames = animationKey["WALK"]["maxFrames"];
      }
      if (animationKey.contains("ATTACKING")) {
        ac.attacking.spriteSheetY = animationKey["ATTACKING"]["spriteSheetY"];
        ac.attacking.maxFrames = animationKey["ATTACKING"]["maxFrames"];
      }

      if (animationKey.contains("CLIMBING")) {
        ac.climbing.spriteSheetY = animationKey["CLIMBING"]["spriteSheetY"];
        ac.climbing.maxFrames = animationKey["CLIMBING"]["maxFrames"];
      }

      if (animationKey.contains("DYING")) {
        ac.dying.spriteSheetY = animationKey["DYING"]["spriteSheetY"];
        ac.dying.maxFrames = animationKey["DYING"]["maxFrames"];
      }

      if (animationKey.contains("AIRBORNE")) {
        ac.airborne.spriteSheetY = animationKey["AIRBORNE"]["spriteSheetY"];
        ac.airborne.maxFrames = animationKey["AIRBORNE"]["maxFrames"];
      }

      if (animationKey.contains("PUSH")) {
        ac.pushing.spriteSheetY = animationKey["PUSH"]["spriteSheetY"];
        ac.pushing.maxFrames = animationKey["PUSH"]["maxFrames"];
      }

      if (animationKey.contains("STILL")) {
        ac.still.spriteSheetY = animationKey["STILL"]["spriteSheetY"];
        ac.still.maxFrames = animationKey["STILL"]["maxFrames"];
      }

      if (animationKey.contains("WALK")) {
        ac.walking.spriteSheetY = animationKey["WALK"]["spriteSheetY"];
        ac.walking.maxFrames = animationKey["WALK"]["maxFrames"];
      }

      if (animationKey.contains("SHOOTING")) {
        ac.shooting.spriteSheetY = animationKey["SHOOTING"]["spriteSheetY"];
        ac.shooting.maxFrames = animationKey["SHOOTING"]["maxFrames"];
      }
    }

    // Decide render list placement by type or optional "renderLayer" in JSON
    std::string t = def.value("type", "");
    if (t == "player" || t == "enemy" || t == "object") {
      level.objectRenderIds.emplace_back(entity);
    } else if (t == "ui") {
      level.UIRenderIds.emplace_back(entity);
    } else {
      // tiles and others -> tileRenderIds
      level.tileRenderIds.emplace_back(entity);
    }
  }

  // Health
  if (def.contains("health")) {
    level.registry.emplace<HealthComponent>(entity, def.value("health", 1));
    // set currentHealth to same value after construction: HealthComponent ctor sets max only
    auto &hc = level.registry.get<HealthComponent>(entity);
    hc.currentHealth = def.value("health", hc.maxHealth);
  }

  // TODO should come fron json
  if (def.contains("attack")) {
    auto attackDef = def["attack"];
    const int px = x;
    const int py = y;
    const SDL_Rect collisionRect = {px + Constants::PLAYER_WIDTH, py, Constants::PLAYER_WIDTH,
                                    Constants::PLAYER_HEIGHT};
    int attackFrames = attackDef.value("attackFrames", 36);
    int attackHitDetectionFrame = attackDef.value("attackHitDetectionFrame", 24);
    int damage = attackDef.value("damage", 1);
    level.registry.emplace<AttackComponent>(entity, attackFrames, attackHitDetectionFrame, damage,
                                            collisionRect);
  }

  if (def.contains("shoot")) {
    auto shootDef = def["shoot"];
    int shootFrames = shootDef.value("shootingFrames", 64);
    int launchFrame = shootDef.value("launchFrame", 32);
    int damage = shootDef.value("damage", 1);
    entt::entity shooterEntity = entity;
    level.registry.emplace<ShootComponent>(entity, shootFrames, launchFrame, damage, shooterEntity);
  }

  // Gravity
  if (def.contains("gravity")) {
    level.registry.emplace<GravityComponent>(entity, GRAVITY_STATE::GROUNDED);
  }

  // Control (for player)
  if (def.contains("control")) {
    level.registry.emplace<ControlComponent>(entity, CONTROL_STATE::NORMAL);
  }

  // AI (for enemy)
  if (def.contains("ai")) {
    level.registry.emplace<AIComponent>(entity);
  }

  if (def.contains("collision")) {
    SDL_Rect collisionRect = {x, y, sprite.sourceRect.w, sprite.sourceRect.h};
    level.registry.emplace<CollisionComponent>(entity, collisionRect);
    auto &layer = def["collision"]["layer"];
    if (layer == 1) {
    }
    if (layer == 3) {
      level.staticCollisionIds.emplace_back(entity);
    }
  }

  if (def.contains("tile")) {
    auto tile = static_cast<TILE>(def.value("tile", 1));
    level.registry.emplace<TileComponent>(entity, tile, x / 128, y / 128);
    level.tileRenderIds.emplace_back(entity);
  }

  // Player entity pointer
  if (def.value("type", "") == "player") {
    level.playerEntity = entity;
  }
}

// --- End: JSON template helpers and template-driven entity creation ---

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
  auto entity = level.registry.create();

  level.registry.emplace<PositionComponent>(entity, x, y);
  level.registry.emplace<TileComponent>(entity, TILE::NONE, x / 128, y / 128);

  SDL_Rect const sourceRect = {128, 384, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);

  level.tileRenderIds.emplace_back(entity);
}

void SceneCreator::createBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.registry.emplace<PositionComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, destinationRect);
  level.registry.emplace<TileComponent>(entity, TILE::SOLID, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createSemiSolidBlockEntity(int x, int y, int sourceX, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.registry.emplace<PositionComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, sourceRect);
  level.registry.emplace<TileComponent>(entity, TILE::SEMISOLID, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createLadder(int x, int y, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {0, 192, 128, 128};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, 128, 128};

  level.registry.emplace<PositionComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<TileComponent>(entity, TILE::LADDER, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createLadderStart(int x, int y, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {0, 192, 128, 64};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {x, y, 128, 64};

  level.registry.emplace<PositionComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<TileComponent>(entity, TILE::LADDERSTART, x / 128, y / 128);

  level.tileRenderIds.emplace_back(entity);
  level.staticCollisionIds.emplace_back(entity);
}

void SceneCreator::createUI(int x, int y, SceneInfo &level) {
  auto entity = level.registry.create();
  SDL_Rect const sourceRect = {0, 0, 31, 31};
  SDL_Rect const destinationRect = {x, y, sourceRect.w, sourceRect.h};

  level.registry.emplace<PositionComponent>(entity, x, y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 2);

  level.UIRenderIds.emplace_back(entity);
}

void SceneCreator::createPC(int x, int y, SceneInfo &level) {
  auto tpl = findTemplateByType("player");
  if (!tpl.is_null()) {
    createEntityFromTemplate(tpl, x, y, level);
    return;
  }
}

void SceneCreator::createEnemy(int x, int y, SceneInfo &level) {
  auto tpl = findTemplateByType("enemy");
  if (!tpl.is_null()) {
    createEntityFromTemplate(tpl, x, y, level);
    return;
  }
}

void SceneCreator::createBlock(std::string const &gameObjectType, int x, int y, SceneInfo &level) {
  // If a JSON block template exists with variant gameObjectType use it.
  auto tpl = findTemplateByType(gameObjectType);
  if (!tpl.is_null()) {
    createEntityFromTemplate(tpl, x, y, level);
    return;
  }

  // fallback to existing logic: B1/B2/B3
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
