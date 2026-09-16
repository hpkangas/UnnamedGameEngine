#ifndef SHOOTCOMPONENT_HPP
#define SHOOTCOMPONENT_HPP
#include "../utilities/utils.hpp"
#include "../include/entt/entt.hpp"

struct ShootComponent {
     explicit ShootComponent(int shootingFrames, int launchFrame, int damage, entt::entity shooterEntity)
      : shootingFrames(shootingFrames),launchFrame(launchFrame), damage(damage), shooterEntity(shooterEntity) {}
  int shootingFrames = 0;
  int launchFrame = 0;
  int damage = 0;
  int currentShootingFrame = 0;
  Utils::Position launchStartPosition = {0, 0};
  Utils::Velocity launchVelocity = {0, 0};
  entt::entity shooterEntity = entt::null;

  bool shooting = false;
};

#endif