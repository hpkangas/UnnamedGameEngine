#include "shootsystem.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/rendercomponent.hpp"
#include <iostream>

void ShootSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<ShootComponent, TransformComponent>();

  for (auto entity : view) {
    ShootComponent &sc = view.get<ShootComponent>(entity);
    TransformComponent const &tc = view.get<TransformComponent>(entity);

    updateShootComponents(sc, tc);

    if (sc.shooting) {
      if (sc.currentShootingFrame == sc.launchFrame) {
        createProjectile(sc);
      }
    }
  }
}

void ShootSystem::updateShootComponents(ShootComponent &shootComponent,
                                        TransformComponent const &transformComponent) const {
  shootComponent.launchStartPosition = transformComponent.position;
}

void ShootSystem::createProjectile(ShootComponent const &shootComponent) const {
  auto entity = currentLevel->createEntity();
  auto &reg = currentLevel->registry;

  reg.emplace<TransformComponent>(entity, shootComponent.launchStartPosition.x,
                                  shootComponent.launchStartPosition.y, 12,
                                  shootComponent.launchVelocity.y);
  reg.emplace<CollisionComponent>(entity, SDL_Rect{shootComponent.launchStartPosition.x,
                                                   shootComponent.launchStartPosition.y, 16, 16});
  reg.emplace<ProjectileComponent>(
      entity, ProjectileComponent{shootComponent.damage, shootComponent.shooterEntity});
  reg.emplace<RenderComponent>(
      entity, SDL_Rect{0, 0, 16, 16},
      SDL_Rect{shootComponent.launchStartPosition.x, shootComponent.launchStartPosition.y, 16, 16},
      SDL_FLIP_NONE, 1);
  currentLevel->objectRenderIds.emplace_back(entity);
  std::cout << "Creating projectile at position (" << shootComponent.launchStartPosition.x << ", "
            << shootComponent.launchStartPosition.y << ") with velocity (" << 5 << ", "
            << shootComponent.launchVelocity.y << ")" << std::endl;
}
