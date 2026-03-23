#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "utilities/utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Camera {
public:
  Camera();
  inline static SDL_Rect position{};
  inline static Utils::Position offset{};

  static void updatePositionRelativeToScreenAndOffset();
  static void updateOffsetRelativeToScreen(int offsetx, int offsety);
};

#endif
