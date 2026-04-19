#include "animationsystem.hpp"
#include "../utilities/constants.hpp"
#include <cstdio>

using namespace Constants;
void AnimationSystem::update() {
  for (int id : currentLevel->animationIds) {
    // what if these don't exist
    CollisionComponent const &collisionComponent = *currentLevel->collisionComponents.at(id);
    GRAVITY_STATE const &gravityState = currentLevel->gravityComponents.at(id)->gravityState;
    TransformComponent const &transformComponent = *currentLevel->transformComponents.at(id);
    RenderComponent &renderComponent = *currentLevel->renderComponents.at(id);
    AnimationComponent &animationComponent = *currentLevel->animationComponents.at(id);

    AttackComponent const *attackComponent = nullptr;
    if (currentLevel->attackComponents.contains(id)) {
      attackComponent = &*currentLevel->attackComponents.at(id);
    }

    ControlComponent const *controlComponent = nullptr;
    if (currentLevel->controlComponents.contains(id)) {
      controlComponent = &*currentLevel->controlComponents.at(id);
    }

    HealthComponent const *healthComponent = nullptr;
    if (currentLevel->healthComponents.contains(id)) {
      healthComponent = &*currentLevel->healthComponents.at(id);
    }

    AIComponent const *aiComponent = nullptr;
    if (currentLevel->aiComponents.contains(id)) {
      aiComponent = &*currentLevel->aiComponents.at(id);
    }

    if (!aiComponent) {
      determineAnimationState(collisionComponent, gravityState, transformComponent,
                              animationComponent, attackComponent, controlComponent,
                              healthComponent);
    } else {
      determineAIAnimationState(collisionComponent, gravityState, transformComponent,
                                animationComponent, aiComponent);
    }

    resetAnimation(animationComponent);

    animate(renderComponent, animationComponent, transformComponent);
  }
}

void AnimationSystem::resetAnimation(AnimationComponent &animationComponent) const {
  if (previousAnimationState != animationComponent.animationState) {
    animationComponent.animationFrame = 0;
  }
}

void AnimationSystem::determineAnimationState(CollisionComponent const &collisionComponent,
                                              GRAVITY_STATE const gravityState,
                                              TransformComponent const &transformComponent,
                                              AnimationComponent &animationComponent,
                                              AttackComponent const *attackComponent,
                                              ControlComponent const *controlComponent,
                                              HealthComponent const *healthComponent) {

  previousAnimationState = animationComponent.animationState;

  if (controlComponent != nullptr) {

    if (controlComponent->controlState == CONTROL_STATE::ATTACKING) {
      animationComponent.animationState = ANIMATION_STATE::ATTACKING;
      return;
    }
  }

  if (healthComponent != nullptr && healthComponent->healthState == HEALTH_STATE::DEAD) {
    animationComponent.animationState = ANIMATION_STATE::DEATH;
    return;
  }

  if (gravityState == GRAVITY_STATE::GROUNDED) {
    if (transformComponent.velocity.x != 0 && transformComponent.velocity.y == 0) {
      animationComponent.animationState = ANIMATION_STATE::WALKING;
    } else {
      animationComponent.animationState = ANIMATION_STATE::STILL;
    }

    if (collisionComponent.collisionLeft || collisionComponent.collisionRight) {
      animationComponent.animationState = ANIMATION_STATE::PUSHING;
    }

    // doesn't work when you stop
    // climbing needs a lot of rework
    if (transformComponent.velocity.y != 0) {
      animationComponent.animationState = ANIMATION_STATE::CLIMBING;
    }

  } else {
    animationComponent.animationState = ANIMATION_STATE::AIRBORNE;
  }
}

void AnimationSystem::determineAIAnimationState(CollisionComponent const &collisionComponent,
                                                GRAVITY_STATE const gravityState,
                                                TransformComponent const &transformComponent,
                                                AnimationComponent &animationComponent,
                                                AIComponent const *aiComponent) {
  previousAnimationState = animationComponent.animationState;

  if (aiComponent->state == AI_STATE::ATTACKING) {
    animationComponent.animationState = ANIMATION_STATE::ATTACKING;
    return;
  }

  if (aiComponent->state == AI_STATE::DEAD) {
    animationComponent.animationState = ANIMATION_STATE::DEATH;
    return;
  }

  if (gravityState == GRAVITY_STATE::GROUNDED) {
    if (transformComponent.velocity.x != 0 && transformComponent.velocity.y == 0) {
      animationComponent.animationState = ANIMATION_STATE::WALKING;
    } else {
      animationComponent.animationState = ANIMATION_STATE::STILL;
    }

    if (collisionComponent.collisionLeft || collisionComponent.collisionRight) {
      animationComponent.animationState = ANIMATION_STATE::PUSHING;
    }

    if (transformComponent.velocity.y != 0) {
      animationComponent.animationState = ANIMATION_STATE::CLIMBING;
    }

  } else {
    animationComponent.animationState = ANIMATION_STATE::AIRBORNE;
  }
}

void AnimationSystem::animate(RenderComponent &renderComponent,
                              AnimationComponent &animationComponent,
                              TransformComponent const &transformComponent) const {
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

  case ANIMATION_STATE::ATTACKING:
    playFrame(renderComponent, 0, ATTACKING_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 6);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 6 > 5) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::DEATH:
    playFrame(renderComponent, 0, DYING_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 12);

    if (animationComponent.animationFrame / 12 < 3) {
      animationComponent.animationFrame++;
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
