#ifndef SHOOTCOMPONENT_HPP
#define SHOOTCOMPONENT_HPP
#include "../utilities/utils.hpp"
#include "../include/entt/entt.hpp"
#include "rendercomponent.hpp"

struct ShootComponent {
     explicit ShootComponent(int shootingFrames, int launchFrame, int damage, entt::entity shooterEntity, RenderInfo projectileRender)
      : shootingFrames(shootingFrames),launchFrame(launchFrame), damage(damage), shooterEntity(shooterEntity), projectileRender(projectileRender) {}
  int shootingFrames = 0;
  int launchFrame = 0;
  int damage = 0;
  int currentShootingFrame = 0;
  entt::entity shooterEntity = entt::null;

  Utils::Position launchPosition = {0, 0};
  Utils::Velocity launchVelocity = {0, 0};

  RenderInfo projectileRender = {};

  bool shooting = false;
};

#endif