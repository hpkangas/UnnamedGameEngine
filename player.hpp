#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "constants.hpp"
#include "gameobject.hpp"
#include <string>

class Player : public GameObject {
private:
  std::string m_name = "";
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
  SDL_Texture *m_objTexture;

  void gravity();
  void handleMovement();
  void handleDownCollision();
  void handleHorizontalCollision();
  void handleUpCollision(bool collision, int dif);
  void handleGravityState();
  void handleAnimationState();

public:
  Player(SDL_Texture *texture, int x, int y, std::string name);
  Player();
  ~Player();

  SDL_Rect m_sourceRect, m_destinationRect, m_collisionRect;

  void playFrame(int x, int y, int w, int h, int frame);

  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
  void stopX();
  void stopYU();
  void stopYD();

  void update();
  void render(Camera camera);

  const int getXpos() { return m_position.x; }

  const int getYpos() { return m_position.y; }

  const float getXvel() { return m_velocity.x; }

  const float getYvel() { return m_velocity.y; }

  void setXpos(int xpos) { m_position.x = xpos; }

  void setYpos(int ypos) { m_position.y = ypos; }

  void setXvel(float xvel) { m_velocity.x = xvel; }

  void setYvel(float yvel) { m_velocity.y = yvel; }

  const int getXposDifferenceWithCollisionObject() { return m_xposDif; }

  const int getYposDifferenceWithCollisionObject() { return m_yposDif; }

  const bool getCollisionDown() { return m_collisionDown; }

  const bool getCollisionUp() { return m_collisionUp; }

  const bool getCollisionRight() { return m_collisionRight; }

  const bool getCollisionLeft() { return m_collisionLeft; }

  void setCollisionDown(bool collision) { m_collisionDown = collision; }

  void setCollisionUp(bool collision) { m_collisionUp = collision; }

  void setCollisionRight(bool collision) { m_collisionRight = collision; }

  void setCollisionLeft(bool collision) { m_collisionLeft = collision; }

  void setXposDifferenceWithCollisionObject(int xposDif) { m_xposDif = xposDif; }

  void setYposDifferenceWithCollisionObject(int yposDif) { m_yposDif = yposDif; }

  const GravityState getGravityState() { return m_gravityState; }

  void setGravityState(GravityState gravityState) { m_gravityState = gravityState; }

  const AnimationState getAnimationState() { return m_animationState; }

  void setAnimationState(AnimationState animationState) { m_animationState = animationState; }

  const std::string getName() { return m_name; }
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
