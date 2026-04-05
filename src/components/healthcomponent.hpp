#pragma once
#ifndef HEALTHCOMPONENT_HPP
#define HEALTHCOMPONENT_HPP

struct HealthComponent {
  explicit HealthComponent(int health) : health(health) {}
  int health;
};

#endif
