#include "healthsystem.hpp"
#include <iostream>

void HealthSystem::update() {
  for (int id : currentLevel->healthIds) {
    HealthComponent &healthComponent = *currentLevel->healthComponents.at(id);
    healthComponent.currentHealth -= healthComponent.damageTaken;
    healthComponent.damageTaken = 0;
    std::cout << "id " << id << ", current health " << healthComponent.currentHealth
              << ", damage taken " << healthComponent.damageTaken << std::endl;
    if (healthComponent.currentHealth <= 0) {
      std::cout << "id " << id << " died\n";
      healthComponent.healthState = HEALTH_STATE::DYING;
    }
  }
}
