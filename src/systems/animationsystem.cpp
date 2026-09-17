#include "animationsystem.hpp"
#include "../utilities/constants.hpp"
#include <cstdio>

using namespace Constants;
void AnimationSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<AnimationComponent, CollisionComponent, GravityComponent, TransformComponent,
                       RenderComponent>();
  for (auto entity : view) {
    AnimationComponent &animationComponent = view.get<AnimationComponent>(entity);
    CollisionComponent const &collisionComponent = view.get<CollisionComponent>(entity);
    GRAVITY_STATE const &gravityState = view.get<GravityComponent>(entity).gravityState;
    TransformComponent const &transformComponent = view.get<TransformComponent>(entity);
    RenderComponent &renderComponent = view.get<RenderComponent>(entity);

    AttackComponent const *attackComponent = reg.try_get<AttackComponent>(entity);
    ControlComponent const *controlComponent = reg.try_get<ControlComponent>(entity);
    HealthComponent const *healthComponent = reg.try_get<HealthComponent>(entity);
    AIComponent const *aiComponent = reg.try_get<AIComponent>(entity);

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
  if (animationComponent.previousAnimationState != animationComponent.animationState) {
    animationComponent.animationFrame = 0;
  }
}

void AnimationSystem::determineAnimationState(CollisionComponent const &collisionComponent,
                                              GRAVITY_STATE const gravityState,
                                              TransformComponent const &transformComponent,
                                              AnimationComponent &animationComponent,
                                              AttackComponent const *attackComponent,
                                              ControlComponent const *controlComponent,
                                              HealthComponent const *healthComponent) const {

  animationComponent.previousAnimationState = animationComponent.animationState;

  if (controlComponent != nullptr) {
    if (controlComponent->controlState == CONTROL_STATE::ATTACKING) {
      animationComponent.animationState = ANIMATION_STATE::ATTACKING;
      return;
    }
    if (controlComponent->controlState == CONTROL_STATE::SHOOTING) {
      animationComponent.animationState = ANIMATION_STATE::SHOOTING;
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
    // TODO triggers when airborne
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
  animationComponent.previousAnimationState = animationComponent.animationState;

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
    playFrame(renderComponent, 0, animationComponent.stillFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame);
    break;

  case ANIMATION_STATE::WALKING:
    playFrame(renderComponent, 0, animationComponent.walkingFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.walkingFrame.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.walkingFrame.maxFrames > animationComponent.walkingFrame.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::PUSHING:
    playFrame(renderComponent, 0, animationComponent.pushingFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.pushingFrame.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.pushingFrame.maxFrames > animationComponent.pushingFrame.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::AIRBORNE:
    playFrame(renderComponent, 0, animationComponent.inAirFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.inAirFrame.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.inAirFrame.maxFrames > animationComponent.inAirFrame.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::CLIMBING:
    playFrame(renderComponent, 0, animationComponent.climbingFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.climbingFrame.maxFrames);
    if (transformComponent.velocity.y < 0) {
      animationComponent.animationFrame++;
      if (animationComponent.animationFrame / animationComponent.climbingFrame.maxFrames > animationComponent.climbingFrame.maxFrames - 1) {
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
    playFrame(renderComponent, 0, animationComponent.attackingFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / 6);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 6 > 5) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::SHOOTING:
    playFrame(renderComponent, 0, animationComponent.shootingFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.shootingFrame.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.shootingFrame.maxFrames > animationComponent.shootingFrame.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::DEATH:
    playFrame(renderComponent, 0, animationComponent.dyingFrame.spriteSheetX, PLAYER_WIDTH, PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.dyingFrame.maxFrames);

    if (animationComponent.animationFrame / animationComponent.dyingFrame.maxFrames < animationComponent.dyingFrame.maxFrames / 4) {
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
