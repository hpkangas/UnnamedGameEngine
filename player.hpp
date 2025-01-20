#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "constants.hpp"
#include "gameobject.hpp"
#include <string>

class Player : public GameObject {
  std::string m_name;
  Position m_position = {0, 0};
  Velocity m_velocity = {0, 0};

  float m_playerSpeed = 0;
  float m_playerSpeedMax = 0;

  int m_yposDif = 0;
  int m_xposDif = 0;

  bool m_collisionUp = false;
  bool m_collisionDown = false;
  bool m_collisionLeft = false;
  bool m_collisionRight = false;

  int m_animationFrame = 0;
  int m_WALKING_ANIMATION_FRAMES = 0;

  GravityState m_gravityState;
  AnimationState m_animationState;

  SDL_RendererFlip m_flipType;
  SDL_Texture *m_objTexture{};

  void gravity();
  void handleMovement();
  void handleDownCollision();
  void handleHorizontalCollision();
  void handleUpCollision(bool collision, int dif);
  void handleGravityState();
  void handleAnimationState();

public:
  Player(SDL_Texture &texture, int x, int y, const std::string &name);
  Player();
  ~Player() override;

  SDL_Rect m_sourceRect{}, m_destinationRect{}, m_collisionRect{};

  void playFrame(int x, int y, int w, int h, int frame);

  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
  void stopX();
  void stopYU();
  void stopYD();

  void update() override;
  void render(Camera camera) override;

  int getXpos() const override { return m_position.x; }

  int getYpos() const override { return m_position.y; }

  float getXvel() const { return m_velocity.x; }

  float getYvel() const { return m_velocity.y; }

  void setXpos(const int xpos) override { m_position.x = xpos; }

  void setYpos(const int ypos) override { m_position.y = ypos; }

  void setXvel(const float xvel) { m_velocity.x = xvel; }

  void setYvel(const float yvel) { m_velocity.y = yvel; }

  int getXposDifferenceWithCollisionObject() const { return m_xposDif; }

  int getYposDifferenceWithCollisionObject() const { return m_yposDif; }

  bool getCollisionDown() const { return m_collisionDown; }

  bool getCollisionUp() const { return m_collisionUp; }

  bool getCollisionRight() const { return m_collisionRight; }

  bool getCollisionLeft() const { return m_collisionLeft; }

  void setCollisionDown(const bool collision) { m_collisionDown = collision; }

  void setCollisionUp(const bool collision) { m_collisionUp = collision; }

  void setCollisionRight(const bool collision) { m_collisionRight = collision; }

  void setCollisionLeft(const bool collision) { m_collisionLeft = collision; }

  void setXposDifferenceWithCollisionObject(const int xposDif) { m_xposDif = xposDif; }

  void setYposDifferenceWithCollisionObject(const int yposDif) { m_yposDif = yposDif; }

  GravityState getGravityState() const { return m_gravityState; }

  void setGravityState(const GravityState gravityState) { m_gravityState = gravityState; }

  AnimationState getAnimationState() const { return m_animationState; }

  void setAnimationState(const AnimationState animationState) { m_animationState = animationState; }

  std::string getName() { return m_name; }
};

// totally not final names
class Baelog : public Player {
public:
  using Player::Player;
  std::string name = "Baelog";
};

class Erik : public Player {
public:
  using Player::Player;
  std::string name = "Erik";
};

class Olaf : public Player {
public:
  using Player::Player;
  std::string name = "Olaf";
};

#endif
