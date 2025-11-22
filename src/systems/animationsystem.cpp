#include "animationsystem.hpp"
#include "../constants.hpp"
#include <cstdio>

void AnimationSystem::determineAnimationState(ControlComponent const &controlComponent,
                                              CollisionComponent const &collisionComponent,
                                              GRAVITY_STATE gravityState,
                                              TransformComponent const &transformComponent,
                                              RenderComponent &renderComponent,
                                              AnimationComponent &animationComponent) {
  switch (controlComponent.getControlState()) {
  case CONTROL_STATE::NORMAL:
    if (transformComponent.getXvel() != 0) {
      animationComponent.setAnimationState(ANIMATION_STATE::WALKING);
    } else {
      animationComponent.setAnimationState(ANIMATION_STATE::STILL);
    }
    if (collisionComponent.getCollisionLeft() || collisionComponent.getCollisionRight()) {
      animationComponent.setAnimationState(ANIMATION_STATE::PUSHING);
    }
    break;
  case CONTROL_STATE::AIRBORNE:
    animationComponent.setAnimationState(ANIMATION_STATE::AIRBORNE);
    break;
  case CONTROL_STATE::FALLING:
    break;
  case CONTROL_STATE::CLIMBING:
    animationComponent.setAnimationState(ANIMATION_STATE::CLIMBING);
    break;
  }
}

void AnimationSystem::update(ControlComponent const &controlComponent,
                             CollisionComponent const &collisionComponent,
                             GRAVITY_STATE gravityState,
                             TransformComponent const &transformComponent,
                             RenderComponent &renderComponent,
                             AnimationComponent &animationComponent) {

  determineAnimationState(controlComponent, collisionComponent, gravityState, transformComponent,
                          renderComponent, animationComponent);

  switch (animationComponent.getAnimationState()) {
  case ANIMATION_STATE::STILL:
    animationComponent.animationFrame = 0;
    playFrame(renderComponent, 0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame);
    break;

  case ANIMATION_STATE::WALKING:

    playFrame(renderComponent, 0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.WALKING_ANIMATION_FRAMES);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.WALKING_ANIMATION_FRAMES > 7) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::PUSHING:

    playFrame(renderComponent, 0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 8);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 8 > 7) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::AIRBORNE:

    playFrame(renderComponent, 0, INAIR_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 8);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 8 > 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::CLIMBING:

    playFrame(renderComponent, 0, CLIMBING_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 8);
    if (transformComponent.getYvel() < 0) {
      animationComponent.animationFrame++;
      if (animationComponent.animationFrame / 5 > 5) {
        animationComponent.animationFrame = 0;
      }
    }
    if (transformComponent.getYvel() > 0) {
      animationComponent.animationFrame--;
      if (animationComponent.animationFrame < 0) {
        animationComponent.animationFrame = 30;
      }
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
