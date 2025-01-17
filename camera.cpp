#include "camera.hpp"
#include "constants.hpp"

Camera::Camera()
{
    m_offset = {0, 0, 0, 0};
    m_position = {SCREENWIDTH * 1 / 3, SCREENHEIGHT * 1 / 3, SCREENWIDTH * 2 / 3, SCREENHEIGHT * 2 / 3};
}

void Camera::updatePositionRelativeToScreenAndOffset()
{
    m_position.x = SCREENWIDTH * 1 / 3 + m_offset.x;
    m_position.w = SCREENWIDTH * 2 / 3 + m_offset.x;
    m_position.y = SCREENHEIGHT * 1 / 3 + m_offset.y;
    m_position.h = SCREENHEIGHT * 2 / 3 + m_offset.y;
}

void Camera::updateOffsetRelativeToScreen(int offsetx, int offsety){
    m_offset.x = offsetx - SCREENWIDTH * 1 / 2;
    m_offset.y = offsety - SCREENHEIGHT * 1 / 2;
}
