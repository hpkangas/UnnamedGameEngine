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
  focusArea.left = SCREENWIDTH * 1 / 3 + offset.x;
  focusArea.top = SCREENHEIGHT * 1 / 3 + offset.y;
  focusArea.right = SCREENWIDTH * 2 / 3 + offset.x;
  focusArea.bottom = SCREENHEIGHT * 2 / 3 + offset.y;

  viewPort = {offset.y, SCREENHEIGHT + offset.y, offset.x, SCREENWIDTH + offset.x};
}

void Camera::updateOffsetRelativeToScreen(const int x, const int y) {
  offset.x = x - SCREENWIDTH * 1 / 2;
  offset.y = y - SCREENHEIGHT * 1 / 2;
}
