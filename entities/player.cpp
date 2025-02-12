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

  // TODO to rendersystem
  renderComponent.destinationRect.x = transformComponent.position.x;
  renderComponent.destinationRect.y = transformComponent.position.y;

  // TODO sprite offsets. should be defined inside the specific character
  // classes (once decided on)

  // TODO to collisionsystem
  collisionComponent.collisionRect.x = transformComponent.position.x + 28;
  collisionComponent.collisionRect.y = transformComponent.position.y;
}

void Player::handleUpCollision(bool collision, int dif) {
  // TODO
}
