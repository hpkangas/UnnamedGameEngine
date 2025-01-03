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

    static bool playerCollisionDown(Player player, const Block obj)
    {

        // Tämä if omaan funktioon?
        if (player.collisionRect.y + player.collisionRect.h <= obj.collisionRect.y)
        {
            {
                return (player.collisionRect.y + player.collisionRect.h + player.getYvel() >= obj.collisionRect.y &&
                        player.collisionRect.x < obj.collisionRect.x + obj.collisionRect.w - 1 &&
                        player.collisionRect.x + player.collisionRect.w - 1 > obj.collisionRect.x);
            }
        }
    }

    static bool playerCollisionUp(Player player, const Block obj)
    {
        if (player.collisionRect.y >= obj.collisionRect.y + obj.collisionRect.h)
        {

            return (player.collisionRect.y + player.getYvel() <= obj.collisionRect.y + obj.collisionRect.h &&
                    player.collisionRect.x < obj.collisionRect.x + obj.collisionRect.w &&
                    player.collisionRect.x + player.collisionRect.w - 1 > obj.collisionRect.x);
        }
    }
    static bool playerCollisionLeft(Player player, const Block obj)
    {
        if (player.collisionRect.x >= obj.collisionRect.x + obj.collisionRect.w)
        {

            return (player.collisionRect.x + player.getXvel() < obj.collisionRect.x + obj.collisionRect.w &&
                    player.collisionRect.y < obj.collisionRect.y + obj.collisionRect.h - 1 &&
                    player.collisionRect.y + player.collisionRect.h - 1 > obj.collisionRect.y);
        }
    }

    //jotain hassua tässä, pelaaja työntyy eteenpäin ja palaa takaisin
    static bool playerCollisionRight(Player player, const Block obj)
    {
        if (player.collisionRect.x + player.collisionRect.w <= obj.collisionRect.x)
        {

            return (player.collisionRect.x + player.collisionRect.w + player.getXvel() > obj.collisionRect.x &&
                    player.collisionRect.y < obj.collisionRect.y + obj.collisionRect.h - 1 &&
                    player.collisionRect.y + player.collisionRect.h - 1 > obj.collisionRect.y);
        }
    }
};
#endif