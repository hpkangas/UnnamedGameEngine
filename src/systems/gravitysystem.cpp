#include "gravitysystem.hpp"
#include "../components/controlcomponent.hpp"
#include <iostream>

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
    auto down = collisionComponent->collisionTileTypeDown;
    auto up = collisionComponent->collisionTileTypeUp;
    if (down == TILE::EMPTY || down == TILE::LADDER) {
      gravityComponent.gravityState = GRAVITY_STATE::AIRBORNE;
    }

    if (up == TILE::LADDER &&
        down == TILE::SOLID) {
      gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
    }

    if (down == TILE::SOLID ||
        down == TILE::LADDERSTART) {
      gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
    }
  }
  
  if (reg.any_of<ControlComponent>(entity)) {
    controlComponent = &reg.get<ControlComponent>(entity);
    if (controlComponent && controlComponent->controlState == CONTROL_STATE::CLIMBING) {
      gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
    }
    if (controlComponent && controlComponent->controlState == CONTROL_STATE::AIRBORNE) {
      gravityComponent.gravityState = GRAVITY_STATE::AIRBORNE;
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
