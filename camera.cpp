#include "camera.hpp"
#include "constants.hpp"

Camera::Camera(){
    this->offset = {0, 0, 0, 0};
    this->position = {SCREENWIDTH * 1/4, SCREENHEIGHT + 100, SCREENWIDTH * 3/4, SCREENHEIGHT - 100};
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
