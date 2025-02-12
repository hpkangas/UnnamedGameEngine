#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <vector>

class Entity {
public:
  std::vector<IComponent> components;
  void add(IComponent component);

  TransformComponent transformComponent;
  RenderComponent renderComponent;
  CollisionComponent collisionComponent;
  GravityComponent gravityComponent;
  AnimationComponent animationComponent;
};