#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "utilities/utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Bounds {
  int top;
  int bottom;
  int left;
  int right;
};

class Camera {
public:
  Camera();
  inline static Bounds focusArea{};
  inline static Bounds viewPort{};
  inline static Utils::Position offset{};

  static void update();
  static void updateOffsetRelativeToScreen(int x, int y);
};

#endif
