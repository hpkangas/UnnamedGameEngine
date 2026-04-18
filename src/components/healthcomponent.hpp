#pragma once
#ifndef HEALTHCOMPONENT_HPP
#define HEALTHCOMPONENT_HPP

enum class HEALTH_STATE { DEAD = 0, ALIVE = 1 };

struct HealthComponent {
  explicit HealthComponent(int maxHealth) : maxHealth(maxHealth) {}
  HEALTH_STATE healthState = HEALTH_STATE::ALIVE;
  int maxHealth = 0;
  int currentHealth = maxHealth;
  int damageTaken = 0;
};

#endif
