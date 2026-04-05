#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "utilities/utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Camera {
public:
  Camera();
  inline static SDL_Rect focusArea{};
  inline static SDL_Rect viewPort{};
  inline static Utils::Position offset{};

  static void update();
  static void updateOffsetRelativeToScreen(int offsetx, int offsety);
};

#endif
