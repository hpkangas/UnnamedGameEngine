#pragma once
#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP
#include "../utils.hpp"
#include "component.hpp"
#include <iostream>

using namespace Utils;
struct TransformComponent : public IComponent {
private:
  int ID = TCID;
  Position position{0, 0};
  Velocity velocity{0, 0};

public:
  // jump component?
  int jumpTics = -1;
  // read from the end.. because of reasons
  int jumpTable[3] = {2, 4, 6};

  TransformComponent() {}
  ~TransformComponent() {}

  int getXpos() const { return position.x; }

  int getYpos() const { return position.y; }

  Position getPosition() { return position; }
  void setPosition(Position position) { this->position = position; }

  void setXpos(const int xpos) { position.x = xpos; }

  void setYpos(const int ypos) { position.y = ypos; }

  float getXvel() const { return velocity.x; }

  float getYvel() const { return velocity.y; }

  Velocity getVelocity() { return velocity; }
  void setVelocity(Velocity velocity) { this->velocity = velocity; }

  void setXvel(const float xvel) { velocity.x = xvel; }

  void setYvel(const float yvel) {
    if (yvel != 0) {
      std::cout << yvel << std::endl;
    }
    velocity.y = yvel;
  }
};

#endif
