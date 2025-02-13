#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../components/animationcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <string>

class Player {

  std::string m_name;

  void handleUpCollision(bool collision, int dif);

public:
  Player(int x, int y, const std::string &name);
  Player() {};
  ~Player() {};

  TransformComponent transformComponent{};
  RenderComponent renderComponent{};
  CollisionComponent collisionComponent{};
  AnimationComponent animationComponent{};
  GravityComponent gravityComponent{};

  std::string getName() { return m_name; }
};

// totally not final names
class Baleog : public Player {
public:
  using Player::Player;
};

class Erik : public Player {
public:
  using Player::Player;
};

class Olaf : public Player {
public:
  using Player::Player;
};

#endif
