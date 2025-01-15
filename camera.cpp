#include "camera.hpp"
#include "constants.hpp"

Camera::Camera()
{
    this->m_offset = {0, 0, 0, 0};
    this->m_position = {SCREENWIDTH * 1 / 3, SCREENHEIGHT * 1 / 3, SCREENWIDTH * 2 / 3, SCREENHEIGHT * 2 / 3};
}

void Camera::updateCameraPosition()
{
    this->m_position.x = SCREENWIDTH * 1 / 3 + this->m_offset.x;
    this->m_position.w = SCREENWIDTH * 2 / 3 + this->m_offset.x;
    this->m_position.y = SCREENHEIGHT * 1 / 3 + this->m_offset.y;
    this->m_position.h = SCREENHEIGHT * 2 / 3 + this->m_offset.y;
}
