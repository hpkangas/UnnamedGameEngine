#ifndef PROJECTILECOMPONENT_HPP
#define PROJECTILECOMPONENT_HPP
#include "../include/entt/entt.hpp"
#include <SDL_rect.h>


struct ProjectileComponent {
  int damage = 0;
  entt::entity shooterEntity = entt::null;
  bool collided = false;
};

#endif