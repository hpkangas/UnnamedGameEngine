#include "healthsystem.hpp"

void HealthSystem::update() {
  for (int id : currentLevel->healthIds) {
    HealthComponent &healthComponent = currentLevel->healthComponents.at(id);
    healthComponent.currentHealth -= healthComponent.damageTaken;
    healthComponent.damageTaken = 0;
    if (healthComponent.currentHealth <= 0) {
      healthComponent.healthState = HEALTH_STATE::DEAD;
    }
  }
}
