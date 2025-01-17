#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class
    Camera
{
public:
    Camera();
    SDL_Rect m_position;
    //offset doesn't need to be sdl_rect. vector from texturemanager? --> utils?
    SDL_Rect m_offset;

    void updatePositionRelativeToScreenAndOffset();
    void updateOffsetRelativeToScreen(int offsetx, int offsety);
};

#endif