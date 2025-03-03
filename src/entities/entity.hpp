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
  Entity();
  ~Entity() {}
  std::vector<std::unique_ptr<IComponent>> components;
  void add(std::unique_ptr<IComponent> component);

  // don't touch
  template <typename T> T *GetComponent() {
    for (auto &c : components) {
      if (T *tc = dynamic_cast<T *>(c.get())) {
        return tc;
      }
    }
    return nullptr;
  }

  TransformComponent *getTransformComponent();
  RenderComponent *getRenderComponent();
  CollisionComponent *getCollisionComponent();
  GravityComponent *getGravityComponent();
  AnimationComponent *getAnimationComponent();
  int ID = -1;
  int getID();
};

#endif
