#include "gravitysystem.hpp"

constexpr float GRAVITY_UNIT = 0.75f;
constexpr float MAX_GRAVITY = 11.0;

void GravitySystem::update() const {
  auto &reg = currentLevel->registry;
  auto view = reg.view<GravityComponent>();
  for (auto entity : view) {
    auto &gc = view.get<GravityComponent>(entity);
    auto const &cc = reg.get<CollisionComponent>(entity);
    auto &velocityComponent = reg.get<VelocityComponent>(entity);
    updateGravity(cc, gc, velocityComponent);
  }
}

void GravitySystem::updateGravity(CollisionComponent const &collisionComponent,
                                  GravityComponent &gravityComponent,
                                  VelocityComponent &velocityComponent) const {

  if (collisionComponent.collisionTileDown == TILE::LADDERSTART ||
      collisionComponent.collisionTileDown == TILE::SOLID) {
    gravityComponent.gravityState = GRAVITY_STATE::GROUNDED;
  } else {
    gravityComponent.gravityState = GRAVITY_STATE::AIRBORNE;
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
