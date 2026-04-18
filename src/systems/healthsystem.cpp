#include "healthsystem.hpp"
#include <iostream>

void HealthSystem::update() {
  for (int id : currentLevel->healthIds) {
    auto &healthComponent = currentLevel->healthComponents.at(id);
    healthComponent->currentHealth -= healthComponent->damageTaken;
    healthComponent->damageTaken = 0;
    if (healthComponent->currentHealth <= 0) {
      healthComponent->healthState = HEALTH_STATE::DYING;
    }
  }
}
