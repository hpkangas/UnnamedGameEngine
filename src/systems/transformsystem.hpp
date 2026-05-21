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
  SceneInfo *currentLevel = nullptr;

  void updateAll() const;
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
