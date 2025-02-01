#include "player.hpp"
#include "constants.hpp"
#include "texturemanager.hpp"
#include <SDL2/SDL_render.h>

Player::Player(const int x, const int y, const std::string &name) {
  m_name = name;
  renderComponent.m_objTexture = TextureManager::LoadTexture("assets/baleog.png");
  transformComponent.m_position = {x, y};

  m_flipType = SDL_FLIP_NONE;

  renderComponent.m_sourceRect = {0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT};

  renderComponent.m_destinationRect = {x, y, renderComponent.m_sourceRect.w,
                                       renderComponent.m_sourceRect.h};

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  m_collisionRect = {transformComponent.m_position.x + 28, transformComponent.m_position.y,
                     PLAYER_WIDTH - 56, PLAYER_HEIGHT};

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

void Player::update() {
  handleDownCollision();
  handleHorizontalCollision();
  handleGravityState();
  handleMovement();
  handleAnimationState();

  renderComponent.m_destinationRect.x = transformComponent.m_position.x;
  renderComponent.m_destinationRect.y = transformComponent.m_position.y;

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  m_collisionRect.x = transformComponent.m_position.x + 28;
  m_collisionRect.y = transformComponent.m_position.y;
}

// void Player::render(const Camera camera) {
//   const RenderInfo info = {m_objTexture, rc.m_sourceRect, rc.m_destinationRect, m_flipType,
//   camera}; TextureManager::draw(info);
// }

void Player::playFrame(const int x, const int y, const int w, const int h, const int frame) {
  renderComponent.m_sourceRect = {x + w * frame, y, w, h};
}

void Player::moveRight() {
  if (getAnimationState() != WALKING)
    setAnimationState(WALKING);

  transformComponent.setXvel(transformComponent.getXvel() + m_playerSpeed);

  if (transformComponent.getXvel() > m_playerSpeedMax)
    transformComponent.setXvel(m_playerSpeedMax);
}

void Player::moveLeft() {
  if (getAnimationState() != WALKING)
    setAnimationState(WALKING);

  transformComponent.setXvel(transformComponent.getXvel() - m_playerSpeed);

  if (transformComponent.getXvel() < -m_playerSpeedMax)
    transformComponent.setXvel(-m_playerSpeedMax);
}

void Player::moveUp() { transformComponent.setYvel(-m_playerSpeed); }

// debugging only mostly
void Player::moveDown() { transformComponent.setYvel(m_playerSpeed); }

// debugging only mostly
void Player::stopX() {
  if (transformComponent.getXvel() != 0) {
    transformComponent.setXvel(0);
    setAnimationState(STILL);
  }
}

// debugging only mostly
void Player::stopYU() {
  if (transformComponent.m_velocity.y < 0) {
    transformComponent.m_velocity.y = 0;
    m_animationState = STILL;
  }
}

// debugging only mostly
void Player::stopYD() {
  if (transformComponent.m_velocity.y > 0) {
    transformComponent.m_velocity.y = 0;
    m_animationState = STILL;
  }
}

void Player::gravity() {
  if (transformComponent.getYvel() < MAX_GRAVITY) {
    transformComponent.setYvel(transformComponent.getYvel() + GRAVITYUNIT);
  }
}

void Player::handleMovement() {
  const float xvel = transformComponent.getXvel();
  const float yvel = transformComponent.getYvel();
  const int xpos = transformComponent.getXpos();
  const int ypos = transformComponent.getYpos();

  if (!(xvel == 0)) {
    transformComponent.setXpos(xpos + xvel);
  }

  if (!(yvel == 0)) {
    transformComponent.setYpos(ypos + yvel);
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
      transformComponent.setYpos(transformComponent.getYpos() -
                                 getYposDifferenceWithCollisionObject());
    transformComponent.setYvel(0);
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
      transformComponent.setXpos(transformComponent.getXpos() - xposDif);
    }
    transformComponent.setXvel(0);

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
