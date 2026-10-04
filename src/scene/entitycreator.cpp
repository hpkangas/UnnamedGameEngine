#include "entitycreator.hpp"
#include "../components/aicomponent.hpp"
#include "../components/animationcomponent.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/controlcomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/shootcomponent.hpp"
#include "../components/tilecomponent.hpp"
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
    std::cout << "EntityCreator: failed to open " << dataFilePath << "\n";
    return false;
  }
  try {
    ifs >> g_entityTemplates;
  } catch (const std::exception &e) {
    std::cout << "EntityCreator: json parse error: " << e.what() << std::endl;
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

entt::entity createEntityFromTemplate(const nlohmann::json &def, Position position,
                                      Velocity velocity, SceneInfo &level) {
  if (def.is_null()) {
    return entt::null;
  }

  auto entity = level.registry.create();

  level.registry.emplace<PositionComponent>(entity, position.x, position.y);

  if (def.contains("velocity")) {
    level.registry.emplace<VelocityComponent>(entity, velocity.x, velocity.y);
  }

  // Sprite/render (if present)
  SpriteDef sprite = parseSpriteFromJSON(def);
  if (!sprite.texture.empty()) {
    int texIndex = textureIndexFromName(sprite.texture);
    SDL_Rect src = sprite.sourceRect;
    SDL_Rect dst = {position.x, position.y, src.w, src.h};
    int layer = sprite.layer;
    level.registry.emplace<RenderComponent>(entity, src, dst, SDL_FLIP_NONE, texIndex, layer);
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
  }

  // Health
  if (def.contains("health")) {
    level.registry.emplace<HealthComponent>(entity, def.value("health", 1));
    auto &hc = level.registry.get<HealthComponent>(entity);
    hc.currentHealth = def.value("health", hc.maxHealth);
  }

  if (def.contains("attack")) {
    auto attackDef = def["attack"];
    const int px = position.x;
    const int py = position.y;
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

  if (def.contains("projectile")) {
    level.registry.emplace<ProjectileComponent>(entity);
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
    SDL_Rect collisionRect = {position.x, position.y, sprite.sourceRect.w, sprite.sourceRect.h};
    level.registry.emplace<CollisionComponent>(entity, collisionRect);
  }

  if (def.contains("tile")) {
    auto tile = static_cast<TILE>(def.value("tile", 1));
    level.registry.emplace<TileComponent>(entity, tile, position.x / 128, position.y / 128);
  }

  // Player entity pointer
  if (def.value("type", "") == "player") {
    level.playerEntity = entity;
  }

  return entity;
}

entt::entity EntityCreator::createPC(Position position, SceneInfo &level) {
  position.x = position.x * Constants::TILE_SIZE;
  position.y = position.y * Constants::TILE_SIZE;
  auto tpl = findTemplateByType("player");
  if (!tpl.is_null()) {
    return createEntityFromTemplate(tpl, position, Velocity(0, 0), level);
  }
  return entt::null;
}

entt::entity EntityCreator::createEnemy(Position position, SceneInfo &level) {
  position.x = position.x * Constants::TILE_SIZE;
  position.y = position.y * Constants::TILE_SIZE;
  auto tpl = findTemplateByType("enemy");
  if (!tpl.is_null()) {
    return createEntityFromTemplate(tpl, position, Velocity(0, 0), level);
  }
  return entt::null;
}

entt::entity EntityCreator::createProjectile(std::string templateName, Position position,
                                             Velocity velocity, SceneInfo &level) {
  auto tpl = findTemplateByType(templateName);
  if (!tpl.is_null()) {
    return createEntityFromTemplate(tpl, position, velocity, level);
  }
  return entt::null;
}

void EntityCreator::createBlock(std::string const &gameObjectType, Position position,
                                SceneInfo &level) {
  position.x = position.x * Constants::TILE_SIZE;
  position.y = position.y * Constants::TILE_SIZE;
  auto tpl = findTemplateByType(gameObjectType);
  if (!tpl.is_null()) {
    createEntityFromTemplate(tpl, position, Velocity(0, 0), level);
    return;
  }
}

void EntityCreator::createBlock1(Position position, SceneInfo &level) {
  createBlockEntity(position, 0, level);
}
void EntityCreator::createBlock2(Position position, SceneInfo &level) {
  createBlockEntity(position, 128, level);
}
void EntityCreator::createBlock3(Position position, SceneInfo &level) {
  createBlockEntity(position, 256, level);
}

void EntityCreator::createEmptyTile(Position position, SceneInfo &level) {
  auto entity = level.registry.create();

  level.registry.emplace<PositionComponent>(entity, position.x, position.y);
  level.registry.emplace<TileComponent>(entity, TILE::NONE, position.x / 128, position.y / 128);

  SDL_Rect const sourceRect = {128, 384, 128, 128};
  SDL_Rect const destinationRect = {position.x, position.y, sourceRect.w, sourceRect.h};
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
}

void EntityCreator::createBlockEntity(Position position, int sourceX, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {position.x, position.y, sourceRect.w, sourceRect.h};

  level.registry.emplace<PositionComponent>(entity, position.x, position.y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, destinationRect);
  level.registry.emplace<TileComponent>(entity, TILE::SOLID, position.x / 128, position.y / 128);
}

void EntityCreator::createSemiSolidBlockEntity(Position position, int sourceX, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {sourceX, 0, 128, 128};
  SDL_Rect const destinationRect = {position.x, position.y, sourceRect.w, sourceRect.h};
  level.registry.emplace<PositionComponent>(entity, position.x, position.y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, sourceRect);
  level.registry.emplace<TileComponent>(entity, TILE::SEMISOLID, position.x / 128,
                                        position.y / 128);
}

void EntityCreator::createLadder(Position position, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {0, 192, 128, 128};
  SDL_Rect const destinationRect = {position.x, position.y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {position.x, position.y, 128, 128};

  level.registry.emplace<PositionComponent>(entity, position.x, position.y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<TileComponent>(entity, TILE::LADDER, position.x / 128, position.y / 128);
}

void EntityCreator::createLadderStart(Position position, SceneInfo &level) {
  auto entity = level.registry.create();

  SDL_Rect const sourceRect = {0, 192, 128, 64};
  SDL_Rect const destinationRect = {position.x, position.y, sourceRect.w, sourceRect.h};
  SDL_Rect const collisionRect = {position.x, position.y, 128, 64};

  level.registry.emplace<PositionComponent>(entity, position.x, position.y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 1);
  level.registry.emplace<CollisionComponent>(entity, collisionRect);
  level.registry.emplace<TileComponent>(entity, TILE::LADDERSTART, position.x / 128,
                                        position.y / 128);
}

void EntityCreator::createUI(Position position, SceneInfo &level) {
  auto entity = level.registry.create();
  SDL_Rect const sourceRect = {0, 0, 31, 31};
  SDL_Rect const destinationRect = {position.x, position.y, sourceRect.w, sourceRect.h};

  level.registry.emplace<PositionComponent>(entity, position.x, position.y);
  level.registry.emplace<RenderComponent>(entity, sourceRect, destinationRect, SDL_FLIP_NONE, 2);
}
