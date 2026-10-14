#include "healthsystem.hpp"
#include "../components/healthcomponent.hpp"

void HealthSystem::update() {
  auto view = currentLevel->registry.view<HealthComponent>();
  for (auto entity : view) {
    HealthComponent &h = view.get<HealthComponent>(entity);
    h.currentHealth -= h.damageTaken;
    h.damageTaken = 0;
    if (h.currentHealth <= 0) {
      h.healthState = HEALTH_STATE::DEAD;
      currentLevel->destroyEntity(entity);
    }
  }
}
