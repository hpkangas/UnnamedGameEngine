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
  Position m_position{0, 0};
  Velocity m_velocity{0, 0};
  TransformComponent() {};
  ~TransformComponent() {}
  int getXpos() const { return m_position.x; }

  int getYpos() const { return m_position.y; }

  void setXpos(const int xpos) { m_position.x = xpos; }

  void setYpos(const int ypos) { m_position.y = ypos; }

  float getXvel() const { return m_velocity.x; }

  float getYvel() const { return m_velocity.y; }

  void setXvel(const float xvel) { m_velocity.x = xvel; }

  void setYvel(const float yvel) { m_velocity.y = yvel; }
};

#endif