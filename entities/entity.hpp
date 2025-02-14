#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <memory>
#include <vector>

class Entity {
public:
  std::vector<std::unique_ptr<IComponent>> components;
  void add(std::unique_ptr<IComponent> component);
  int ID = -1;
  int getID();

  TransformComponent transformComponent{};
  RenderComponent renderComponent{};
  CollisionComponent collisionComponent{};
  GravityComponent gravityComponent{};
  AnimationComponent animationComponent;
};

#endif