#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../components/collisioncomponent.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include "../constants.hpp"
#include <string>

class Player {

  std::string m_name;

  float m_playerSpeed = 0;
  float m_playerSpeedMax = 0;

  int m_animationFrame = 0;
  int m_WALKING_ANIMATION_FRAMES = 0;

  GravityState m_gravityState;
  AnimationState m_animationState;

  void gravity();
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
  void playFrame(int x, int y, int w, int h, int frame);

  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
  void stopX();
  void stopYU();
  void stopYD();

  void update();

  // void render(Camera camera) override;

  // int getXpos() const override { return m_position.x; }

  // int getYpos() const override { return m_position.y; }

  // float getXvel() const { return m_velocity.x; }

  // float getYvel() const { return m_velocity.y; }

  // void setXpos(const int xpos) override { m_position.x = xpos; }

  // void setYpos(const int ypos) override { m_position.y = ypos; }

  // void setXvel(const float xvel) { m_velocity.x = xvel; }

  // void setYvel(const float yvel) { m_velocity.y = yvel; }



  GravityState getGravityState() const { return m_gravityState; }

  void setGravityState(const GravityState gravityState) { m_gravityState = gravityState; }

  AnimationState getAnimationState() const { return m_animationState; }

  void setAnimationState(const AnimationState animationState) { m_animationState = animationState; }

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
