#ifndef SHOOTCOMPONENT_HPP
#define SHOOTCOMPONENT_HPP  

struct ShootComponent {
     explicit ShootComponent(int attackFrame, int launchFrame, int damage)
      : shootingFrames(attackFrame),launchFrame(launchFrame), damage(damage) {}
  int shootingFrames = 0;
  int launchFrame = 0;
  int damage = 0;
  int currentShootingFrames = 0;

  bool shooting = false;
};

#endif