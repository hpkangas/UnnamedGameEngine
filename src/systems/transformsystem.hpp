#ifndef TRANSFORMSYSTEM_HPP
#define TRANSFORMSYSTEM_HPP
#include "../components/positioncomponent.hpp"
#include "../components/velocitycomponent.hpp"
#include "../scene/sceneinfo.hpp"

class TransformSystem {

  //TODO get from json

public:
  SceneInfo *currentLevel = nullptr;
  
  void updateAll() const;
  void update(VelocityComponent &velocityComponent, PositionComponent &positionComponent) const;
};

  void moveRight(VelocityComponent &velocityComponent, PositionComponent &positionComponent) ;
  void moveLeft(VelocityComponent &velocityComponent, PositionComponent &positionComponent) ;
  void moveUp(VelocityComponent &velocityComponent) ;
  void moveDown(VelocityComponent &velocityComponent) ;
  void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent) ;
  void stopX(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int xposDif) ;
  void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent) ;
  void stopY(VelocityComponent &velocityComponent, PositionComponent &positionComponent, int yposDif) ;
  void jump(VelocityComponent &velocityComponent) ;
  void resetJump(VelocityComponent &velocityComponent) ;
#endif
