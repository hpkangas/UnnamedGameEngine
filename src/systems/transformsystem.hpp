#ifndef TRANSFORMSYSTEM_HPP
#define TRANSFORMSYSTEM_HPP
#include "../components/transformcomponent.hpp"
#include "../scene/sceneinfo.hpp"

class TransformSystem {
  float playerJumpSpeed = -16.0f;
  float playerSpeed = 0.4f;
  float playerSpeedMax = 8.0f;
  float playerClimbingSpeedMax = 4.0f;

public:
  // Wired by Scene::init so updateAll() can iterate the transform pool.
  SceneInfo *currentLevel = nullptr;

  // Per-pool driver — replaces the old per-id loop in Scene::update.
  void updateAll() const;

  // Per-component step. Still used by updateAll(); ControlSystem also
  // calls moveLeft/moveRight/stopX/etc. on a single component.
  void update(TransformComponent &transformComponent) const;
  void moveRight(TransformComponent &transformComponent) const;
  void moveLeft(TransformComponent &transformComponent) const;
  void moveUp(TransformComponent &transformComponent) const;
  void moveDown(TransformComponent &transformComponent) const;
  void stopX(TransformComponent &transformComponent) const;
  void stopX(TransformComponent &transformComponent, int xposDif) const;
  void stopY(TransformComponent &transformComponent) const;
  void stopY(TransformComponent &transformComponent, int yposDif) const;
  void jump(TransformComponent &transformComponent) const;
  void resetJump(TransformComponent &transformComponent) const;
};
#endif
