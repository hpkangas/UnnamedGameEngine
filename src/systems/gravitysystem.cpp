#include "gravitysystem.hpp"
#include "../components/controlcomponent.hpp"

constexpr float GRAVITY_UNIT = 0.75f;
constexpr float MAX_GRAVITY = 11.0;

void GravitySystem::update() const {
  auto &reg = currentLevel->registry;
  auto view = reg.view<GravityComponent, VelocityComponent>();
  for (auto entity : view) {
    auto &gravityComponent = view.get<GravityComponent>(entity);
    auto &velocityComponent = view.get<VelocityComponent>(entity);
    updateGravity(gravityComponent, velocityComponent, entity);
  }
}

void GravitySystem::updateGravity(GravityComponent &gravityComponent,
                                  VelocityComponent &velocityComponent, entt::entity entity) const {
  auto &reg = currentLevel->registry;

  CollisionComponent *collisionComponent = nullptr;
  ControlComponent *controlComponent = nullptr;

  if (reg.any_of<CollisionComponent>(entity)) {
    collisionComponent = &reg.get<CollisionComponent>(entity);

    const auto &downTileTypes = collisionComponent->collidedDownTileTypes;
    const auto &upTileTypes = collisionComponent->collidedUpTileTypes;

    if (downTileTypes.hasTileType(TILE::LADDER) && downTileTypes.hasTileType(TILE::SOLID)) {
      gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
    } else if (downTileTypes.hasTileType(TILE::SOLID) ||
               downTileTypes.hasTileType(TILE::LADDERSTART)) {
      gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
    } else {
      gravityComponent.gravityState = GRAVITY_STATE::AIRBORNE;
    }
  }

  if (reg.any_of<ControlComponent>(entity)) {
    controlComponent = &reg.get<ControlComponent>(entity);
    if (controlComponent && controlComponent->controlState == CONTROL_STATE::CLIMBING) {
      gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
    }
  }

  switch (gravityComponent.gravityState) {
  case GRAVITY_STATE::GROUNDED:
    break;

  case GRAVITY_STATE::AIRBORNE:
    gravity(velocityComponent);
    break;

  default:
    break;
  }
}

void GravitySystem::gravity(VelocityComponent &velocityComponent) const {
  if (velocityComponent.velocity.y < MAX_GRAVITY) {
    velocityComponent.velocity.y = velocityComponent.velocity.y + GRAVITY_UNIT;
  }
}
