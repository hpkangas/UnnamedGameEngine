#include "animationsystem.hpp"
#include "../components/attackcomponent.hpp"
#include "../components/collisioncomponent.hpp"
#include "../components/gravitycomponent.hpp"
#include "../components/healthcomponent.hpp"
#include "../components/transformcomponent.hpp"
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

void AnimationSystem::resetAnimation(AnimationComponent &animationComponent) const {
  if (animationComponent.previousAnimationState != animationComponent.animationState) {
    animationComponent.animationFrame = 0;
  }
}

void AnimationSystem::determineAnimationState(AnimationComponent &animationComponent,
                                              entt::entity entity) const {

  animationComponent.previousAnimationState = animationComponent.animationState;
  auto &reg = currentLevel->registry;
  CollisionComponent const *collisionComponent = reg.try_get<CollisionComponent>(entity);
  GRAVITY_STATE gravityState = reg.get<GravityComponent>(entity).gravityState;
  TransformComponent const *transformComponent = reg.try_get<TransformComponent>(entity);
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
      animationComponent.animationState = ANIMATION_STATE::ATTACKING;
      return;
    }
  }

  if (gravityState == GRAVITY_STATE::GROUNDED) {
    if (transformComponent->velocity.x != 0 && transformComponent->velocity.y == 0) {
      animationComponent.animationState = ANIMATION_STATE::WALKING;
    } else {
      animationComponent.animationState = ANIMATION_STATE::STILL;
    }

    if (collisionComponent->collisionLeft || collisionComponent->collisionRight) {
      animationComponent.animationState = ANIMATION_STATE::PUSHING;
    }

    // doesn't work when you stop
    // climbing needs a lot of rework
    // TODO triggers when airborne
    if (transformComponent->velocity.y != 0) {
      animationComponent.animationState = ANIMATION_STATE::CLIMBING;
    }

  } else {
    animationComponent.animationState = ANIMATION_STATE::AIRBORNE;
  }
}

void AnimationSystem::animate(AnimationComponent &animationComponent, entt::entity entity) const {
  auto &reg = currentLevel->registry;
  RenderComponent &renderComponent = reg.get<RenderComponent>(entity);
  TransformComponent const &transformComponent = reg.get<TransformComponent>(entity);

  switch (animationComponent.animationState) {
  case ANIMATION_STATE::STILL:
    animationComponent.animationFrame = 0;
    playFrame(renderComponent, 0, animationComponent.still.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT, animationComponent.animationFrame);
    break;

  case ANIMATION_STATE::WALKING:
    playFrame(renderComponent, 0, animationComponent.walking.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.walking.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.walking.maxFrames >
        animationComponent.walking.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::PUSHING:
    playFrame(renderComponent, 0, animationComponent.pushing.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.pushing.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.pushing.maxFrames >
        animationComponent.pushing.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::AIRBORNE:
    playFrame(renderComponent, 0, animationComponent.airborne.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.airborne.maxFrames);
    animationComponent.animationFrame++;
    // TODO fix magic number
    if (animationComponent.animationFrame / animationComponent.airborne.maxFrames > 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::CLIMBING:
    playFrame(renderComponent, 0, animationComponent.climbing.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.climbing.maxFrames);
    if (transformComponent.velocity.y < 0) {
      animationComponent.animationFrame++;
      if (animationComponent.animationFrame / animationComponent.climbing.maxFrames >
          animationComponent.climbing.maxFrames - 1) {
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
    playFrame(renderComponent, 0, animationComponent.attacking.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.attacking.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.attacking.maxFrames >
        animationComponent.attacking.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::SHOOTING:
    playFrame(renderComponent, 0, animationComponent.shooting.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
              animationComponent.animationFrame / animationComponent.shooting.maxFrames);
    animationComponent.animationFrame++;
    if (animationComponent.animationFrame / animationComponent.shooting.maxFrames >
        animationComponent.shooting.maxFrames - 1) {
      animationComponent.animationFrame = 0;
    }
    break;

  case ANIMATION_STATE::DEATH:
    playFrame(renderComponent, 0, animationComponent.dying.spriteSheetY, PLAYER_WIDTH,
              PLAYER_HEIGHT,
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

void AnimationSystem::playFrame(RenderComponent &renderComponent, const int x, const int y,
                                const int w, const int h, const int frame) const {
  renderComponent.sourceRect = {x + w * frame, y, w, h};
}
