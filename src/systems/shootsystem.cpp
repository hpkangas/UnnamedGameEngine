#include "shootsystem.hpp"
#include "../components/projectilecomponent.hpp"
#include "../scene/entitycreator.hpp"

void ShootSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<ShootComponent, PositionComponent, VelocityComponent>();

  for (auto entity : view) {
    ShootComponent &sc = view.get<ShootComponent>(entity);
    PositionComponent const &tc = view.get<PositionComponent>(entity);
    VelocityComponent const &vc = view.get<VelocityComponent>(entity);

    updateShootComponents(sc, tc, vc);

    if (sc.shooting) {
      if (sc.currentShootingFrame == sc.launchFrame) {
        createProjectile(sc);
      }
    }
  }
}

void ShootSystem::updateShootComponents(ShootComponent &shootComponent,
                                        PositionComponent const &positionComponent,
                                        VelocityComponent const &velocityComponent) const {

  // TODO data from json
  if (positionComponent.facingRight) {
    shootComponent.launchVelocity = {44.0f, velocityComponent.velocity.y};
    shootComponent.launchPosition = {positionComponent.position.x + 61,
                                     positionComponent.position.y + 57};
  } else {
    shootComponent.launchVelocity = {-20.0f, velocityComponent.velocity.y};
    shootComponent.launchPosition = {positionComponent.position.x,
                                     positionComponent.position.y + 57};
  }
}

void ShootSystem::createProjectile(ShootComponent const &shootComponent) const {
  // EntityCreator creator;
  auto projectileEntity = EntityCreator::createProjectile(
      "arrow", shootComponent.launchPosition, shootComponent.launchVelocity, *currentLevel);
  auto &projectileComponent = currentLevel->registry.get<ProjectileComponent>(projectileEntity);
  projectileComponent.shooterEntity = shootComponent.shooterEntity;
  projectileComponent.damage = shootComponent.damage;
}
