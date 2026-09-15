#ifndef SHOOTCOMPONENT_HPP
#define SHOOTCOMPONENT_HPP
#include "../utilities/utils.hpp"

struct ShootComponent {
     explicit ShootComponent(int attackFrame, int launchFrame, int damage)
      : shootingFrames(attackFrame),launchFrame(launchFrame), damage(damage) {}
  Utils::Position launchStartPosition = {0, 0};
  Utils::Velocity launchVelocity = {0, 0};
  int shootingFrames = 0;
  int launchFrame = 0;
  int damage = 0;
  int currentShootingFrame = 0;

  bool shooting = false;
};

#endif