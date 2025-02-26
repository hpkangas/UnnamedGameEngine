#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Camera {
public:
  Camera();
  ~Camera() {}
  SDL_Rect m_position{};
  Utils::Position m_offset{};

  void updatePositionRelativeToScreenAndOffset();
  void updateOffsetRelativeToScreen(int offsetx, int offsety);
};

#endif
