// #include "entity.hpp"
// #include <iostream>

// Entity::Entity() { ID = getID(); }

// void Entity::add(std::unique_ptr<IComponent> component) {
//   components.emplace_back(std::move(component));
// }

// int Entity::getID() {
//   static int id = -1;
//   id++;
//   return id;
// }

// TransformComponent *Entity::getTransformComponent() { return GetComponent<TransformComponent>(); }

// // not working, refactor like above?
// RenderComponent *Entity::getRenderComponent() {
//   for (auto &component : components) {
//     if (component->ID == RCID) {
//       return static_cast<RenderComponent *>(component.get());
//     }
//   }
//   std::cout << "No RenderComponent for id " << ID << "\n";
//   return nullptr;
// }

// CollisionComponent *Entity::getCollisionComponent() {
//   for (auto &component : components) {
//     if (component->ID == CCID) {
//       return static_cast<CollisionComponent *>(component.get());
//     }
//   }
//   std::cout << "No Collisioncomponent for id " << ID << "\n";
//   return nullptr;
// }

// GravityComponent *Entity::getGravityComponent() {
//   for (auto &component : components) {
//     if (component->ID == GCID) {
//       return static_cast<GravityComponent *>(component.get());
//     }
//   }
//   std::cout << "No Gravitycomponent for id " << ID << "\n";
//   return nullptr;
// }

// AnimationComponent *Entity::getAnimationComponent() {
//   for (auto &component : components) {
//     if (component->ID == ACID) {
//       return static_cast<AnimationComponent *>(component.get());
//     }
//   }
//   std::cout << "No Animationcomponent for id " << ID << "\n";
//   return nullptr;
// }
