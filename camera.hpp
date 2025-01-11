#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
struct Camera
{
    // Camera();
    static SDL_Rect position;
    static SDL_Rect offset;
    static int xpos;
    static float xvel;
    static int ypos;
    static int yvel;

    static SDL_Rect getPos();

    static int getXpos();

    static int getYpos();

    static void setXpos(int xpos);

    static void setYpos(int ypos);

    static void update();
};

#endif