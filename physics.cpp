#include <SDL2/SDL.h>
#include <iostream>
#include "player.hpp"
#include "gameobject.hpp"
#include "block.hpp"
#ifndef PHYSICS_CPP
#define PHYSICS_CPP

struct Physics
{

    static bool checkCollision(SDL_Rect &a, SDL_Rect b)
    {
        // The sides of the rectangles
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        // Calculate the sides of rect A
        leftA = a.x;
        rightA = a.x + a.w;
        topA = a.y;
        bottomA = a.y + a.h;

        // Calculate the sides of rect B
        leftB = b.x;
        rightB = b.x + b.w;
        topB = b.y;
        bottomB = b.y + b.h;

        // If any of the sides from A are outside of B
        if (bottomA <= topB)
        {
            a.y = topB;
            return false;
        }

        if (topA >= bottomB)
        {
            return false;
        }

        if (rightA <= leftB)
        {
            return false;
        }

        if (leftA >= rightB)
        {
            return false;
        }

        // If none of the sides from A are outside B
        return true;
    }
    static bool checkLeft(SDL_Rect a, SDL_Rect b)
    {
        int leftA, rightB;
        leftA = a.x;
        rightB = b.x + b.w;
        if (leftA >= rightB)
        {
            return false;
        }
        return true;
    }

    static bool playerCollisionDown(const Player player, const Block obj)
    {

        // Tämä if omaan funktioon?
        if (player.collisionRect.y + player.collisionRect.h <= obj.collisionRect.y)
        {

            {

                return (player.collisionRect.y + player.collisionRect.h + 1 + (int)player.yvel >= obj.collisionRect.y &&
                        player.collisionRect.x < obj.collisionRect.x + obj.collisionRect.w - 1 &&
                        player.collisionRect.x + player.collisionRect.w - 1 > obj.collisionRect.x);
            }
        }
    }

    static bool playerCollisionUp(const Player player, const Block obj)
    {
        if (player.collisionRect.y >= obj.collisionRect.y + obj.collisionRect.h)
        {

            return (player.collisionRect.y + player.yvel <= obj.collisionRect.y + obj.collisionRect.h &&
                    player.collisionRect.x < obj.collisionRect.x + obj.collisionRect.w &&
                    player.collisionRect.x + player.collisionRect.w - 1 > obj.collisionRect.x);
        }
    }
    static bool playerCollisionLeft(const Player player, const Block obj)
    {
        if (player.collisionRect.x >= obj.collisionRect.x + obj.collisionRect.w)
        {

            return (player.collisionRect.x + (int)player.xvel <= obj.collisionRect.x + obj.collisionRect.w - 1 &&
                    player.collisionRect.y < obj.collisionRect.y + obj.collisionRect.h - 1 &&
                    player.collisionRect.y + player.collisionRect.h - 1 > obj.collisionRect.y);
        }
    }

    static bool playerCollisionRight(const Player player, const Block obj)
    {
        if (player.collisionRect.x + player.collisionRect.w <= obj.collisionRect.x)
        {

            return (player.collisionRect.x + player.collisionRect.w + (int)player.xvel >= obj.collisionRect.x &&
                    player.collisionRect.y < obj.collisionRect.y + obj.collisionRect.h - 1 &&
                    player.collisionRect.y + player.collisionRect.h - 1 > obj.collisionRect.y);
        }
    }
};
#endif