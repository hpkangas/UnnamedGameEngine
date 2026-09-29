#include "animationsystem.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../utilities/constants.hpp"
#include <cstdio>

using namespace Constants;
void AnimationSystem::update() {
  auto &reg = currentLevel->registry;
  auto view = reg.view<AnimationComponent>();
  for (auto entity : view) {

    AnimationComponent &animationComponent = view.get<AnimationComponent>(entity);

    determineAnimationState(animationComponent, entity);

    resetAnimation(animationComponent);

    animate(animationComponent, entity);
  }
}

void AnimationSystem::determineAnimationState(AnimationComponent &animationComponent,
                                              entt::entity entity) const {

  animationComponent.previousAnimationState = animationComponent.animationState;
  auto &reg = currentLevel->registry;
  CollisionComponent const *collisionComponent = reg.try_get<CollisionComponent>(entity);
  GRAVITY_STATE gravityState = reg.get<GravityComponent>(entity).gravityState;
  PositionComponent const *positionComponent = reg.try_get<PositionComponent>(entity);
  VelocityComponent const *velocityComponent = reg.try_get<VelocityComponent>(entity);
  AttackComponent const *attackComponent = reg.try_get<AttackComponent>(entity);
  ShootComponent const *shootComponent = reg.try_get<ShootComponent>(entity);
  HealthComponent const *healthComponent = reg.try_get<HealthComponent>(entity);

  if (healthComponent != nullptr && healthComponent->healthState == HEALTH_STATE::DEAD) {
    animationComponent.animationState = ANIMATION_STATE::DEATH;
    return;
  }

  if (attackComponent != nullptr) {
    if (attackComponent->attacking) {
      animationComponent.animationState = ANIMATION_STATE::ATTACKING;
      return;
    }
  }

  if (shootComponent != nullptr) {
    if (shootComponent->shooting) {
      animationComponent.animationState = ANIMATION_STATE::SHOOTING;
      return;
    }
  }

  if (gravityState == GRAVITY_STATE::GROUNDED) {
    if (velocityComponent->velocity.x != 0 && velocityComponent->velocity.y == 0) {
      animationComponent.animationState = ANIMATION_STATE::WALKING;
    } else {
      animationComponent.animationState = ANIMATION_STATE::STILL;
    }

    if (collisionComponent->tileCollisionLeft || collisionComponent->tileCollisionRight) {
      animationComponent.animationState = ANIMATION_STATE::PUSHING;
    }

    // doesn't work when you stop
    // climbing needs a lot of rework
    // TODO triggers when airborne
    if (velocityComponent->velocity.y != 0) {
      animationComponent.animationState = ANIMATION_STATE::CLIMBING;
    }

  } else {
    animationComponent.animationState = ANIMATION_STATE::AIRBORNE;
  }
}

void AnimationSystem::animate(AnimationComponent &animationComponent, entt::entity entity) const {
  auto &reg = currentLevel->registry;
  RenderComponent &renderComponent = reg.get<RenderComponent>(entity);
  PositionComponent const &positionComponent = reg.get<PositionComponent>(entity);
  VelocityComponent const &velocityComponent = reg.get<VelocityComponent>(entity);

  switch (animationComponent.animationState) {
  case ANIMATION_STATE::STILL:
    playFrame(renderComponent, animationComponent.still.spriteSheetY,
              animationComponent.animationFrame);
    resetAnimationFrame(animationComponent);
    break;

  case ANIMATION_STATE::WALKING:
    playFrame(renderComponent, animationComponent.walking.spriteSheetY,
              animationComponent.animationFrame / 6);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 6 == animationComponent.walking.maxFrames) {
      resetAnimationFrame(animationComponent);
    }
    break;

  case ANIMATION_STATE::PUSHING:
    playFrame(renderComponent, animationComponent.pushing.spriteSheetY,
              animationComponent.animationFrame / animationComponent.pushing.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.pushing.maxFrames ==
        animationComponent.pushing.maxFrames) {
      resetAnimationFrame(animationComponent);
    }
    break;

  case ANIMATION_STATE::AIRBORNE:
    playFrame(renderComponent, animationComponent.airborne.spriteSheetY,
              animationComponent.animationFrame / animationComponent.airborne.maxFrames);
    animationComponent.animationFrame++;
    // TODO fix magic number
    if (animationComponent.animationFrame / animationComponent.airborne.maxFrames == 1) {
      resetAnimationFrame(animationComponent);
    }
    break;

  case ANIMATION_STATE::CLIMBING:
    playFrame(renderComponent, animationComponent.climbing.spriteSheetY,
              animationComponent.animationFrame / animationComponent.climbing.maxFrames);
    if (velocityComponent.velocity.y < 0) {
      animationComponent.animationFrame++;
      if (animationComponent.animationFrame / animationComponent.climbing.maxFrames ==
          animationComponent.climbing.maxFrames - 1) {
        resetAnimationFrame(animationComponent);
      }
    }
    if (velocityComponent.velocity.y > 0) {
      animationComponent.animationFrame--;
      if (animationComponent.animationFrame < 0) {
        animationComponent.animationFrame = 30;
      }
    }
    break;

  case ANIMATION_STATE::ATTACKING:
    playFrame(renderComponent, animationComponent.attacking.spriteSheetY,
              animationComponent.animationFrame / animationComponent.attacking.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.attacking.maxFrames ==
        animationComponent.attacking.maxFrames) {
      resetAnimationFrame(animationComponent);
    }
    break;

  case ANIMATION_STATE::SHOOTING:
    playFrame(renderComponent, animationComponent.shooting.spriteSheetY,
              animationComponent.animationFrame / 6);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / 6 == animationComponent.shooting.maxFrames) {
      resetAnimationFrame(animationComponent);
    }
    break;

  case ANIMATION_STATE::DEATH:
    playFrame(renderComponent, animationComponent.dying.spriteSheetY,
              animationComponent.animationFrame / animationComponent.dying.maxFrames);

    if (animationComponent.animationFrame / animationComponent.dying.maxFrames <
        animationComponent.dying.maxFrames) {
      animationComponent.animationFrame++;
    }
    break;

  default:
    break;
  }
}


void AnimationSystem::playFrame(RenderComponent &renderComponent, const int y,
                                const int frame) const {
  playFrame(renderComponent, y, renderComponent.sourceRect.w, renderComponent.sourceRect.h,
            frame);
}

void AnimationSystem::playFrame(RenderComponent &renderComponent, const int y,
                                const int w, const int h, const int frame) const {
  renderComponent.sourceRect = { w * frame, y, w, h};
}


void AnimationSystem::resetAnimation(AnimationComponent &animationComponent) const {
  if (animationComponent.previousAnimationState != animationComponent.animationState) {
    resetAnimationFrame(animationComponent);
  }
}

void AnimationSystem::resetAnimationFrame(AnimationComponent &animationComponent) const {
  animationComponent.animationFrame = 0;
}
