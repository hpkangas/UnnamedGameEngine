#pragma once
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "../components/rendercomponent.hpp"
#include "../components/transformcomponent.hpp"
#include <SDL2/SDL_image.h>
#include <string>

class GameObject {
public:
  GameObject() {};
  virtual ~GameObject() {};
  TransformComponent tc{};
  RenderComponent rc{};
  // Position m_position = {0, 0};
  // Velocity m_velocity = {0, 0};

  SDL_Texture *m_objTexture{};
  SDL_Rect m_collisionRect{};

  virtual void update();
  //  virtual void render(Camera camera);
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
  // void render(Camera camera) override;

  std::string text;
};

#endif