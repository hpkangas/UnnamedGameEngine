#include "player.hpp"
#include "../constants.hpp"
#include "../texturemanager.hpp"
#include <SDL2/SDL_render.h>

Player::Player(const int x, const int y, const std::string &name) {
  m_name = name;
  renderComponent.objTexture = TextureManager::LoadTexture("assets/baleog.png");
  transformComponent.position = {x, y};

  renderComponent.flip = SDL_FLIP_NONE;

  renderComponent.sourceRect = {0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT};

  renderComponent.destinationRect = {x, y, renderComponent.sourceRect.w,
                                     renderComponent.sourceRect.h};

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  collisionComponent.collisionRect = {transformComponent.position.x + 28,
                                      transformComponent.position.y, PLAYER_WIDTH - 56,
                                      PLAYER_HEIGHT};

  m_WALKING_ANIMATION_FRAMES = 8;

  m_playerSpeed = 0.4f;
  m_playerSpeedMax = 8.0f;

  gravityComponent.gravityState = GROUNDED;
}

void Player::update() {
  handleDownCollision();
  handleHorizontalCollision();
  handleGravityState();
  handleMovement();
  handleAnimationState();

  renderComponent.destinationRect.x = transformComponent.position.x;
  renderComponent.destinationRect.y = transformComponent.position.y;

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)
  collisionComponent.collisionRect.x = transformComponent.position.x + 28;
  collisionComponent.collisionRect.y = transformComponent.position.y;
}

void Player::playFrame(const int x, const int y, const int w, const int h, const int frame) {
  renderComponent.sourceRect = {x + w * frame, y, w, h};
}

void Player::moveRight() {
  if (animationComponent.getAnimationState() != WALKING)
    animationComponent.setAnimationState(WALKING);

  transformComponent.setXvel(transformComponent.getXvel() + m_playerSpeed);

  if (transformComponent.getXvel() > m_playerSpeedMax)
    transformComponent.setXvel(m_playerSpeedMax);
}

void Player::moveLeft() {
  if (animationComponent.getAnimationState() != WALKING)
    animationComponent.setAnimationState(WALKING);

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
    animationComponent.setAnimationState(STILL);
  }
}

// debugging only mostly
void Player::stopYU() {
  if (transformComponent.velocity.y < 0) {
    transformComponent.velocity.y = 0;
    animationComponent.animationFrame = STILL;
  }
}

// debugging only mostly
void Player::stopYD() {
  if (transformComponent.velocity.y > 0) {
    transformComponent.velocity.y = 0;
    animationComponent.animationFrame = STILL;
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
    renderComponent.flip = SDL_FLIP_HORIZONTAL;
  }

  if (xvel > 0) {
    renderComponent.flip = SDL_FLIP_NONE;
  }
}

void Player::handleDownCollision() {
  if (collisionComponent.getCollisionDown()) {
    if (collisionComponent.getYposDifferenceWithCollisionObject() != 0)
      transformComponent.setYpos(transformComponent.getYpos() -
                                 collisionComponent.getYposDifferenceWithCollisionObject());
    transformComponent.setYvel(0);
    gravityComponent.setGravityState(GROUNDED);
  } else {
    gravityComponent.setGravityState(INAIR);
  }
}

void Player::handleHorizontalCollision() {
  const int xposDif = collisionComponent.getXposDifferenceWithCollisionObject();

  if (!collisionComponent.getCollisionLeft() && !collisionComponent.getCollisionRight()) {
    if (animationComponent.getAnimationState() == PUSHING) {
      animationComponent.setAnimationState(STILL);
    }
  } else {
    if (xposDif != 0) {
      transformComponent.setXpos(transformComponent.getXpos() - xposDif);
    }
    transformComponent.setXvel(0);

    if (gravityComponent.getGravityState() == GROUNDED) {
      animationComponent.setAnimationState(PUSHING);
    }
  }
}

void Player::handleUpCollision(bool collision, int dif) {
  // TODO
}

void Player::handleGravityState() {
  switch (gravityComponent.getGravityState()) {
  case GROUNDED:
    if (animationComponent.getAnimationState() == anims_INAIR)
      animationComponent.setAnimationState(STILL);
    break;

  case INAIR:
    gravity();
    animationComponent.setAnimationState(anims_INAIR);
    break;

  default:
    break;
  }
}

void Player::handleAnimationState() {
  switch (animationComponent.getAnimationState()) {
  case STILL:
    animationComponent.animationFrame = 0;
    playFrame(0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationComponent.animationFrame);
    break;

  case WALKING:
    playFrame(0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / m_WALKING_ANIMATION_FRAMES);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / m_WALKING_ANIMATION_FRAMES > 7) {
      animationComponent.animationFrame = 0;
    }
    break;

  case PUSHING:
    // baelog animaatiossa destination rectille pitäisi olla ehkä joku offset
    playFrame(0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationComponent.animationFrame / 8);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 8 > 7) {
      animationComponent.animationFrame = 0;
    }
    break;

  case anims_INAIR:
    playFrame(0, INAIR_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationComponent.animationFrame / 8);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 8 > 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  default:
    break;
  }
}
