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

  gravityComponent.gravityState = GROUNDED;
}

void Player::update() {
  // TODO handles to systems
  handleGravityState();
  handleAnimationState();

  //TODO to rendersystem
  renderComponent.destinationRect.x = transformComponent.position.x;
  renderComponent.destinationRect.y = transformComponent.position.y;

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)

  //TODO to collisionsystem
  collisionComponent.collisionRect.x = transformComponent.position.x + 28;
  collisionComponent.collisionRect.y = transformComponent.position.y;
}

void Player::playFrame(const int x, const int y, const int w, const int h, const int frame) {
  renderComponent.sourceRect = {x + w * frame, y, w, h};
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
    gravityComponent.gravity(transformComponent);
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
              animationComponent.animationFrame / animationComponent.WALKING_ANIMATION_FRAMES);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.WALKING_ANIMATION_FRAMES > 7) {
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
