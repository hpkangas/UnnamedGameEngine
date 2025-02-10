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

  float m_playerSpeed = 0;
  float m_playerSpeedMax = 0;


  void handleMovement();
  void handleDownCollision();
  void handleHorizontalCollision();
  void handleUpCollision(bool collision, int dif);
  void handleGravityState();
  void handleAnimationState();

public:
  Player(int x, int y, const std::string &name);
  Player() {};
  ~Player() {};

  TransformComponent transformComponent{};
  RenderComponent renderComponent{};
  CollisionComponent collisionComponent{};
  AnimationComponent animationComponent{};
  GravityComponent gravityComponent{};

  void playFrame(int x, int y, int w, int h, int frame);

  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
  void stopX();
  void stopYU();
  void stopYD();

  void update();

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
