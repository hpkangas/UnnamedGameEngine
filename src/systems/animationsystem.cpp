#include "animationsystem.hpp"
#include "../constants.hpp"

void AnimationSystem::update(CollisionComponent const &collisionComponent,
                             GravityState const &gravityState,
                             TransformComponent const &transformComponent,
                             RenderComponent &renderComponent,
                             AnimationComponent &animationComponent) {

  if (gravityState == INAIR) {
    animationComponent.setAnimationState(anims_INAIR);
  }
  else {
    if (transformComponent.getXvel() != 0) {
      animationComponent.setAnimationState(WALKING);
    } else
      animationComponent.setAnimationState(STILL);
    if (collisionComponent.getCollisionLeft() || collisionComponent.getCollisionRight()) {
      animationComponent.setAnimationState(PUSHING);
    }
  }

  switch (animationComponent.getAnimationState()) {
  case STILL:
    animationComponent.animationFrame = 0;
    playFrame(renderComponent, 0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame);
    break;

  case WALKING:
    playFrame(renderComponent, 0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.WALKING_ANIMATION_FRAMES);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.WALKING_ANIMATION_FRAMES > 7) {
      animationComponent.animationFrame = 0;
    }
    break;

  case PUSHING:
    playFrame(renderComponent, 0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 8);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 8 > 7) {
      animationComponent.animationFrame = 0;
    }
    break;

  case anims_INAIR:
    playFrame(renderComponent, 0, INAIR_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 8);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 8 > 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  default:
    break;
  }
}

void AnimationSystem::playFrame(RenderComponent &renderComponent, const int x, const int y,
                                const int w, const int h, const int frame) {
  renderComponent.sourceRect = {x + w * frame, y, w, h};
}