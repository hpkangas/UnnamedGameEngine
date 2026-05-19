#include "healthsystem.hpp"

void HealthSystem::update() {
  // Pool iteration — no id list, no hashing, cache-friendly linear walk.
  for (HealthComponent &h : currentLevel->healthComponents) {
    h.currentHealth -= h.damageTaken;
    h.damageTaken = 0;
    if (h.currentHealth <= 0) {
      h.healthState = HEALTH_STATE::DEAD;
    }
  }
}
