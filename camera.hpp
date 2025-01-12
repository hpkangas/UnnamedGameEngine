#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player;

class
    Camera
{
public:
    SDL_Rect position;
    SDL_Rect offset;

    Camera();
    SDL_Rect getPos();
    SDL_Rect getOffset();
    void setPos(SDL_Rect rect);
    void setOffset(SDL_Rect rect);

    void centerCameraToCharacter(Player *player);
    void updateCameraPosition();
    void updateCamera(Player *player);
};

#endif