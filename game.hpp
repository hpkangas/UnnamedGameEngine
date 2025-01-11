#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "camera.hpp"

class Game
{
public:
    void init(const char *title, int xpos, int ypos, int width, int heigth, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    int getCurrentTime();

    Uint32 currentTime;
    bool running() { return isRunning; }

    static SDL_Renderer *renderer;
    static TTF_Font *gFont;

    Camera camera;

private:
    void moveScreen();

    int count = 0;
    bool isRunning;
    SDL_Window *window;

    void changePlayerCharacter();
    void checkPlayerMovementCollisions();
    void handleInput();
};

#endif /* GAME_HPP */