#include "entity.hpp"

void Entity::add(std::unique_ptr<IComponent> component) {
  components.emplace_back(std::move(component));
};
int Entity::getID() {
  static int id = 0;
  id++;
  return id;
}