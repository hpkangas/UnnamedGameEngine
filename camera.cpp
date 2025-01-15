#include "camera.hpp"
#include "constants.hpp"

Camera::Camera()
{
    this->offset = {0, 0, 0, 0};
    this->position = {SCREENWIDTH * 1 / 3, SCREENHEIGHT * 1 / 3, SCREENWIDTH * 2 / 3, SCREENHEIGHT * 2 / 3};
}

SDL_Rect Camera::getPos()
{
    return this->position;
}

SDL_Rect Camera::getOffset()
{
    return this->offset;
}

void Camera::setPos(SDL_Rect rect)
{
    this->position = rect;
}

void Camera::setOffset(SDL_Rect rect)
{
    this->offset = rect;
}

void Camera::updateCameraPosition()
{
    this->position.x = SCREENWIDTH * 1 / 3 + this->offset.x;
    this->position.w = SCREENWIDTH * 2 / 3 + this->offset.x;
    this->position.y = SCREENHEIGHT * 1 / 3 + this->offset.y;
    this->position.h = SCREENHEIGHT * 2 / 3 + this->offset.y;
}
