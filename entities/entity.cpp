#include "entity.hpp"

void Entity::add(IComponent component) {
    components.emplace_back(component);
};