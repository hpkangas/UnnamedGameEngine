#pragma once
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "camera.hpp"
#include <SDL2/SDL_image.h>
#include <string>

struct Position {
  int x, y;
};

struct Velocity {
  float x, y;
};

class GameObject {
public:
  GameObject() {};
  virtual ~GameObject() {};

  Position m_position = {0, 0};
  Velocity m_velocity = {0, 0};

  SDL_Texture *m_objTexture{};
  SDL_Rect m_sourceRect{}, m_destinationRect{}, m_collisionRect{};

  virtual void update();
  virtual void render(Camera camera);

  virtual int getXpos() const { return m_position.x; }

  virtual int getYpos() const { return m_position.y; }

  virtual void setXpos(const int xpos) { m_position.x = xpos; }

  virtual void setYpos(const int ypos) { m_position.y = ypos; }
};

class Background : public GameObject {
public:
  Background(const char *texturesheet, int x, int y, int sizew, int sizeh);
  void update() override;
};

class GameText : public GameObject {
public:
  GameText(std::string textureText, int x, int y);
  GameText(int x, int y);
  GameText(std::string textureText, int x, int y, int width, int height);
  GameText(int x, int y, int width, int height);
  void update() override;
  void render(Camera camera) override;

  std::string text;
};

#endif