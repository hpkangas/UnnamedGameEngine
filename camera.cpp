#include "camera.hpp"
#include "constants.hpp"

// Camera::Camera(){
//     this->xpos = 0;
//     this->ypos = 0;
// }
SDL_Rect Camera::offset = {0, 0, 0, 0};
SDL_Rect Camera::position = {SCREENWIDTH * 1/4, SCREENHEIGHT + 100, SCREENWIDTH * 3/4, SCREENHEIGHT - 100};
int Camera::xpos = 0;
int Camera::ypos = 0;
float Camera::xvel = 0.0f;
int Camera::yvel = 0;

// SDL_Rect Camera::getPos()
// {
//     return Camera::pos;
// }

int Camera::getXpos()
{
    return Camera::xpos;
}

int Camera::getYpos()
{
    return Camera::ypos;
}

void Camera::setXpos(int xpos)
{
    Camera::xpos = xpos;
}

void Camera::setYpos(int ypos)
{
    Camera::ypos = ypos;
}

void Camera::update()
{
    if (!(xvel == 0))
    {
        Camera::setXpos(xpos + xvel);
    }
}