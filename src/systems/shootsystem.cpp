#include "shootsystem.hpp"
#include "../components/projectilecomponent.hpp"
#include "../components/rendercomponent.hpp"
#include <iostream>

void ShootSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<ShootComponent>();

  for (auto entity : view) {
    ShootComponent &sc = view.get<ShootComponent>(entity);
    PositionComponent const &tc = reg.get<PositionComponent>(entity);
    VelocityComponent const &vc = reg.get<VelocityComponent>(entity);

    updateShootComponents(sc, tc, vc);

    if (sc.shooting) {
      if (sc.currentShootingFrame == sc.launchFrame) {
        createProjectile(sc);
      }
    }
  }
}

void ShootSystem::updateShootComponents(ShootComponent &shootComponent,
                                        PositionComponent const &positionComponent, VelocityComponent const &velocityComponent) const {
  if (positionComponent.facingRight) {
    shootComponent.launchVelocity = {16.0f, velocityComponent.velocity.y};
    shootComponent.launchStartPosition = {positionComponent.position.x,
                                          positionComponent.position.y};
  } else {
    shootComponent.launchStartPosition = {positionComponent.position.x,
                                          positionComponent.position.y};
    shootComponent.launchVelocity = {-16.0f, velocityComponent.velocity.y};
  }
}

void ShootSystem::createProjectile(ShootComponent const &shootComponent) const {
  auto &reg = currentLevel->registry;
  auto entity = reg.create();

  reg.emplace<PositionComponent>(entity, shootComponent.launchStartPosition.x,
                                  shootComponent.launchStartPosition.y);
  reg.emplace<VelocityComponent>(entity, shootComponent.launchVelocity.x,
                                  shootComponent.launchVelocity.y);
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
