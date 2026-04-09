#include "animationsystem.hpp"
#include "../utilities/constants.hpp"
#include <cstdio>

using namespace Constants;
void AnimationSystem::update() const {
for (int id : currentLevel->animationIds) {    
  //what if these don't exist
  CollisionComponent const &collisionComponent = *currentLevel->collisionComponents.at(id);
  GRAVITY_STATE const &gravityState = currentLevel->gravityComponents.at(id)->gravityState;
  TransformComponent const &transformComponent = *currentLevel->transformsComponents.at(id);
  RenderComponent &renderComponent =  *currentLevel->renderComponents.at(id);
  AnimationComponent &animationComponent = *currentLevel->animationComponents.at(id);
    
  determineAnimationState(collisionComponent, gravityState, transformComponent,
                          animationComponent);

  animate(renderComponent, animationComponent, transformComponent);
}
}

void AnimationSystem::determineAnimationState(
                               CollisionComponent const &collisionComponent,
                               GRAVITY_STATE const gravityState,
                               TransformComponent const &transformComponent,
                               AnimationComponent &animationComponent) const {
  if (gravityState == GRAVITY_STATE::GROUNDED) {
    if (transformComponent.velocity.x != 0 && transformComponent.velocity.y == 0) {
      animationComponent.animationState = ANIMATION_STATE::WALKING;
    } else {
      animationComponent.animationState = ANIMATION_STATE::STILL;
    }
    
    if (collisionComponent.collisionLeft || collisionComponent.collisionRight) {
      animationComponent.animationState = ANIMATION_STATE::PUSHING;
    }
    
    //doesn't work when you stop
    //climbing needs a lot of rework
    if (transformComponent.velocity.y != 0) {
      animationComponent.animationState = ANIMATION_STATE::CLIMBING;
    }

  } else {
      animationComponent.animationState = ANIMATION_STATE::AIRBORNE;    
  }
  
}

void AnimationSystem::animate(RenderComponent &renderComponent, AnimationComponent &animationComponent, TransformComponent const &transformComponent) const{
  switch (animationComponent.animationState) {
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
    if (transformComponent.velocity.y < 0) {
      animationComponent.animationFrame++;
      if (animationComponent.animationFrame / 5 > 5) {
        animationComponent.animationFrame = 0;
      }
    }
    if (transformComponent.velocity.y > 0) {
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
                                const int w, const int h, const int frame) const {
  renderComponent.sourceRect = {x + w * frame, y, w, h};
}