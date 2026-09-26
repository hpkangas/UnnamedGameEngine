#ifndef TRANSFORMSYSTEM_HPP
#define TRANSFORMSYSTEM_HPP
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

class TransformSystem {
  float playerJumpSpeed = -16.0f;
  float playerSpeed = 0.4f;
  float playerSpeedMax = 8.0f;
  float playerClimbingSpeedMax = 4.0f;

public:
  SceneInfo *currentLevel = nullptr;

  void updateAll() const;
  void update(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const;
  void moveRight(VelocityComponent &velocityComponent) const;
  void moveLeft(VelocityComponent &velocityComponent) const;
  void moveUp(VelocityComponent &velocityComponent) const;
  void moveDown(VelocityComponent &velocityComponent) const;
  void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const;
  void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int xposDif) const;
  void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const;
  void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int yposDif) const;
  void jump(VelocityComponent &velocityComponent) const;
  void resetJump(VelocityComponent &velocityComponent) const;
};
#endif
