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
  GameObject();
  ~GameObject();

  Position m_position = {0, 0};
  Velocity m_velocity = {0, 0};

  SDL_Texture *m_objTexture;
  SDL_Rect m_sourceRect, m_destinationRect, m_collisionRect;

  virtual void update();
  virtual void render(Camera camera);

  const virtual int getXpos() { return m_position.x; }

  const virtual int getYpos() { return m_position.y; }

  virtual void setXpos(int xpos) { m_position.x = xpos; }

  virtual void setYpos(int ypos) { m_position.y = ypos; }

};

class Background : public GameObject {
public:
  Background(const char *texturesheet, int x, int y, int sizew, int sizeh);
  void update();
};

class GameText : public GameObject {
public:
  GameText(std::string textureText, int x, int y);
  GameText(int x, int y);
  GameText(std::string textureText, int x, int y, int width, int height);
  GameText(int x, int y, int width, int height);
  void update();
  void render(Camera camera);

  std::string text;
};

#endif