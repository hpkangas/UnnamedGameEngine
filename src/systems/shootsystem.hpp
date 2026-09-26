#ifndef SHOOTSYSTEM_HPP
#define SHOOTSYSTEM_HPP
#include "../components/shootcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

struct ShootSystem {
  void update();
  void updateShootComponents(ShootComponent &shootComponent,
                             PositionComponent const &positionComponent, VelocityComponent const &velocityComponent) const;
  void handleShots(ShootComponent const &shootComponent, entt::entity shooterEntity);
  void shoot(ShootComponent const &shootComponent, HealthComponent &healthComponent,
              CollisionComponent const &collisionComponent) const;
  void createProjectile(ShootComponent const &shootComponent) const;

  SceneInfo *currentLevel = nullptr;
};

#endif
