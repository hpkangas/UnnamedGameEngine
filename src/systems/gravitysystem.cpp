#include "gravitysystem.hpp"

constexpr float GRAVITY_UNIT = 0.75f;
constexpr float MAX_GRAVITY = 11.0;

void GravitySystem::gravity(TransformComponent &transformComponent) const {
  if (transformComponent.velocity.y < MAX_GRAVITY) {
    transformComponent.velocity.y = transformComponent.velocity.y + GRAVITY_UNIT;
  }
}

void GravitySystem::update() const {
  // Iterate the gravity pool directly; for every entity with gravity,
  // its collision + transform are mandatory siblings (set up by
  // SceneCreator), so .at() is the right accessor here.
  auto &pool = currentLevel->gravityComponents;
  for (std::size_t i = 0; i < pool.size(); ++i) {
    const int eid = pool.entityAt(i);
    updateOne(currentLevel->collisionComponents.at(eid),
              pool[i],
              currentLevel->transformComponents.at(eid));
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
