#include "shootsystem.hpp"
#include "../components/projectilecomponent.hpp"

void ShootSystem::update(){}

void ShootSystem::createProjectile(ShootComponent const &shootComponent) const {
  auto entity = currentLevel->createEntity();
  auto &reg = currentLevel->registry;

  reg.emplace<TransformComponent>(entity, shootComponent.launchStartPosition.x, shootComponent.launchStartPosition.y);
  reg.emplace<CollisionComponent>(entity, SDL_Rect{shootComponent.launchStartPosition.x, shootComponent.launchStartPosition.y, 16, 16});
  reg.emplace<HealthComponent>(entity, shootComponent.damage);
  reg.emplace<ProjectileComponent>(entity, ProjectileComponent{shootComponent.damage, entt::null});
}
