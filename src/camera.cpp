#include "camera.hpp"
#include "utilities/constants.hpp"

Camera::Camera() {
  offset = {0, 0};
  position = {SCREENWIDTH * 1 / 3, SCREENHEIGHT * 1 / 3, SCREENWIDTH * 2 / 3, SCREENHEIGHT * 2 / 3};
}

void Camera::updatePositionRelativeToScreenAndOffset() {
  position.x = SCREENWIDTH * 1 / 3 + offset.x;
  position.w = SCREENWIDTH * 2 / 3 + offset.x;
  position.y = SCREENHEIGHT * 1 / 3 + offset.y;
  position.h = SCREENHEIGHT * 2 / 3 + offset.y;
}

void Camera::updateOffsetRelativeToScreen(const int offsetx, const int offsety) {
  offset.x = offsetx - SCREENWIDTH * 1 / 2;
  offset.y = offsety - SCREENHEIGHT * 1 / 2;
}
