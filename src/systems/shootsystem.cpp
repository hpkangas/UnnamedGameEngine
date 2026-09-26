#include "shootsystem.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/rendercomponent.hpp"
#include <iostream>

void ShootSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<ShootComponent>();

  for (auto entity : view) {
    ShootComponent &sc = view.get<ShootComponent>(entity);
    TransformComponent const &tc = reg.get<TransformComponent>(entity);

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
  if (transformComponent.facingRight) {
    shootComponent.launchVelocity = {16.0f, transformComponent.velocity.y};
    shootComponent.launchStartPosition = {transformComponent.position.x,
                                          transformComponent.position.y};
  } else {
    shootComponent.launchStartPosition = {transformComponent.position.x,
                                          transformComponent.position.y};
    shootComponent.launchVelocity = {-16.0f, transformComponent.velocity.y};
  }
}

void ShootSystem::createProjectile(ShootComponent const &shootComponent) const {
  auto &reg = currentLevel->registry;
  auto entity = reg.create();

  reg.emplace<TransformComponent>(entity, shootComponent.launchStartPosition.x,
                                  shootComponent.launchStartPosition.y,
                                  shootComponent.launchVelocity.x, shootComponent.launchVelocity.y);
  reg.emplace<CollisionComponent>(entity, SDL_Rect{shootComponent.launchStartPosition.x,
                                                   shootComponent.launchStartPosition.y, 32, 32});
  reg.emplace<ProjectileComponent>(
      entity, ProjectileComponent{shootComponent.damage, shootComponent.shooterEntity});
  reg.emplace<RenderComponent>(
      entity, SDL_Rect{0, 0, 32, 32},
      SDL_Rect{shootComponent.launchStartPosition.x, shootComponent.launchStartPosition.y, 32, 32},
      SDL_FLIP_NONE, 2);
  currentLevel->objectRenderIds.emplace_back(entity);

  std::cout << "Creating projectile at position (" << shootComponent.launchStartPosition.x << ", "
            << shootComponent.launchStartPosition.y << ") with velocity ("
            << shootComponent.launchVelocity.x << ", " << shootComponent.launchVelocity.y << ")"
            << "id " << static_cast<uint32_t>(entity) << std::endl;
}
