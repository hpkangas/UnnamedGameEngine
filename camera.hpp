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
    SDL_Rect m_offset;

    void updateCameraPosition();

    SDL_Rect getPos()
    {
        return m_position;
    }

    SDL_Rect getOffset()
    {
        return m_offset;
    }

    void setPos(SDL_Rect rect)
    {
        m_position = rect;
    }

    void setOffset(SDL_Rect rect)
    {
        m_offset = rect;
    }
};

#endif