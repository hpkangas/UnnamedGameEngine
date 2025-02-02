#pragma once
#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

struct TransformComponent {
  struct Position {
    int x, y;
  };

  struct Velocity {
    float x, y;
  };

  Position position{0, 0};
  Velocity velocity{0, 0};

  TransformComponent() {};
  ~TransformComponent() {}

  int getXpos() const { return position.x; }

  int getYpos() const { return position.y; }

  void setXpos(const int xpos) { position.x = xpos; }

  void setYpos(const int ypos) { position.y = ypos; }

  float getXvel() const { return velocity.x; }

  float getYvel() const { return velocity.y; }

  void setXvel(const float xvel) { velocity.x = xvel; }

  void setYvel(const float yvel) { velocity.y = yvel; }
};

#endif