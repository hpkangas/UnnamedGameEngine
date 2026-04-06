#include "camera.hpp"
#include "utilities/constants.hpp"

using namespace Constants;
Camera::Camera() {
  offset = {0, 0};
  focusArea = {SCREENWIDTH * 1 / 3, SCREENHEIGHT * 1 / 3, SCREENWIDTH * 2 / 3,
               SCREENHEIGHT * 2 / 3};
  viewPort = {0, 0, SCREENWIDTH, SCREENHEIGHT};
}

void Camera::update() {
  focusArea.x = SCREENWIDTH * 1 / 3 + offset.x;
  focusArea.y = SCREENHEIGHT * 1 / 3 + offset.y;
  focusArea.w = SCREENWIDTH * 2 / 3 + offset.x;
  focusArea.h = SCREENHEIGHT * 2 / 3 + offset.y;

  viewPort = {offset.x, offset.y, SCREENWIDTH + offset.x, SCREENHEIGHT + offset.y};
}

void Camera::updateOffsetRelativeToScreen(const int offsetx, const int offsety) {
  offset.x = offsetx - SCREENWIDTH * 1 / 2;
  offset.y = offsety - SCREENHEIGHT * 1 / 2;
}
