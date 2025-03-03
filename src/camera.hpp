#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace Utils;

class Camera {
public:
  Camera();
  ~Camera() {}
  inline static SDL_Rect position{};
  inline static Position offset{};

  static void updatePositionRelativeToScreenAndOffset();
  static void updateOffsetRelativeToScreen(int offsetx, int offsety);
};

#endif
