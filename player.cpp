#include "player.hpp"
#include "constants.hpp"
#include "gameobject.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_render.h>

Player::Player(SDL_Texture &texture, const int x, const int y, const std::string &name) {
  m_name = name;
  m_objTexture = &texture;
  m_position = {x, y};

  m_flipType = SDL_FLIP_NONE;

  m_sourceRect = {0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT};

  m_destinationRect = {x, y, m_sourceRect.w, m_sourceRect.h};

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  m_collisionRect = {m_position.x + 28, m_position.y, PLAYER_WIDTH - 56, PLAYER_HEIGHT};

  m_animationFrame = 1;
  m_WALKING_ANIMATION_FRAMES = 8;

  m_playerSpeed = 0.4f;
  m_playerSpeedMax = 8.0f;
  m_xposDif = 0;
  m_yposDif = 0;

  m_animationState = STILL;
  m_gravityState = GROUNDED;
  m_flipType = SDL_FLIP_NONE;
}

Player::~Player() = default;

void Player::update() {
  handleDownCollision();
  handleHorizontalCollision();
  handleGravityState();
  handleMovement();
  handleAnimationState();

  m_destinationRect.x = m_position.x;
  m_destinationRect.y = m_position.y;

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  m_collisionRect.x = m_position.x + 28;
  m_collisionRect.y = m_position.y;
}

void Player::render(const Camera camera) {
  const RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, m_flipType, camera};
  TextureManager::draw(info);
}

void Player::playFrame(const int x, const int y, const int w, const int h, const int frame) {
  m_sourceRect = {x + w * frame, y, w, h};
}

void Player::moveRight() {
  if (getAnimationState() != WALKING)
    setAnimationState(WALKING);

  setXvel(getXvel() + m_playerSpeed);

  if (getXvel() > m_playerSpeedMax)
    setXvel(m_playerSpeedMax);
}

void Player::moveLeft() {
  if (getAnimationState() != WALKING)
    setAnimationState(WALKING);

  setXvel(getXvel() - m_playerSpeed);

  if (getXvel() < -m_playerSpeedMax)
    setXvel(-m_playerSpeedMax);
}

void Player::moveUp() { setYvel(-m_playerSpeed); }

// debugging only mostly
void Player::moveDown() { setYvel(m_playerSpeed); }

// debugging only mostly
void Player::stopX() {
  if (getXvel() != 0) {
    setXvel(0);
    setAnimationState(STILL);
  }
}

// debugging only mostly
void Player::stopYU() {
  if (m_velocity.y < 0) {
    m_velocity.y = 0;
    m_animationState = STILL;
  }
}

// debugging only mostly
void Player::stopYD() {
  if (m_velocity.y > 0) {
    m_velocity.y = 0;
    m_animationState = STILL;
  }
}

void Player::gravity() {
  if (getYvel() < MAX_GRAVITY) {
    setYvel(getYvel() + GRAVITYUNIT);
  }
}

void Player::handleMovement() {
  const float xvel = getXvel();
  const float yvel = getYvel();
  const int xpos = getXpos();
  const int ypos = getYpos();

  if (!(xvel == 0)) {
    setXpos(xpos + xvel);
  }

  if (!(yvel == 0)) {
    setYpos(ypos + yvel);
  }

  if (xvel < 0) {
    m_flipType = SDL_FLIP_HORIZONTAL;
  }

  if (xvel > 0) {
    m_flipType = SDL_FLIP_NONE;
  }
}

void Player::handleDownCollision() {
  if (getCollisionDown()) {
    if (getYposDifferenceWithCollisionObject() != 0)
      setYpos(getYpos() - getYposDifferenceWithCollisionObject());
    setYvel(0);
    setGravityState(GROUNDED);
  } else {
    setGravityState(INAIR);
  }
}

void Player::handleHorizontalCollision() {
  const int xposDif = getXposDifferenceWithCollisionObject();

  if (!getCollisionLeft() && !getCollisionRight()) {
    if (getAnimationState() == PUSHING) {
      setAnimationState(STILL);
    }
  } else {
    if (xposDif != 0) {
      setXpos(getXpos() - xposDif);
    }
    setXvel(0);

    if (getGravityState() == GROUNDED) {
      setAnimationState(PUSHING);
    }
  }
}

void Player::handleUpCollision(bool collision, int dif) {
  // TODO
}

void Player::handleGravityState() {
  switch (getGravityState()) {
  case GROUNDED:
    if (getAnimationState() == anims_INAIR)
      setAnimationState(STILL);
    break;

  case INAIR:
    gravity();
    setAnimationState(anims_INAIR);
    break;

  default:
    break;
  }
}

void Player::handleAnimationState() {
  switch (getAnimationState()) {
  case STILL:
    m_animationFrame = 0;
    playFrame(0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, m_animationFrame);
    break;

  case WALKING:
    playFrame(0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              m_animationFrame / m_WALKING_ANIMATION_FRAMES);
    m_animationFrame++;
    if (m_animationFrame / m_WALKING_ANIMATION_FRAMES > 7) {
      m_animationFrame = 0;
    }
    break;

  case PUSHING:
    // baelog animaatiossa destination rectille pitäisi olla ehkä joku offset
    playFrame(0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, m_animationFrame / 8);
    m_animationFrame++;
    if (m_animationFrame / 8 > 7) {
      m_animationFrame = 0;
    }
    break;

  case anims_INAIR:
    playFrame(0, INAIR_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, m_animationFrame / 8);
    m_animationFrame++;
    if (m_animationFrame / 8 > 1) {
      m_animationFrame = 0;
    }
    break;

  default:
    break;
  }
}
