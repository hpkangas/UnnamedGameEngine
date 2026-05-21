#include "gravitysystem.hpp"

constexpr float GRAVITY_UNIT = 0.75f;
constexpr float MAX_GRAVITY = 11.0;

void GravitySystem::gravity(TransformComponent &transformComponent) const {
  if (transformComponent.velocity.y < MAX_GRAVITY) {
    transformComponent.velocity.y = transformComponent.velocity.y + GRAVITY_UNIT;
  }
}

void GravitySystem::update() const {
  auto view = currentLevel->registry.view<GravityComponent, CollisionComponent, TransformComponent>();
  for (auto entity : view) {
    auto &gc = view.get<GravityComponent>(entity);
    auto const &cc = view.get<CollisionComponent>(entity);
    auto &tc = view.get<TransformComponent>(entity);
    updateOne(cc, gc, tc);
  }
}

void GravitySystem::updateOne(CollisionComponent const &collisionComponent,
                              GravityComponent &gravityComponent,
                              TransformComponent &transformComponent) const {

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
    gravity(transformComponent);
    break;

  default:
    break;
  }
}
