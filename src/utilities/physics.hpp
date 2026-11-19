#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "utils.hpp"
#include <SDL2/SDL.h>

using namespace Utils;
namespace Physics {

bool checkCollision(SDL_Rect const &a, SDL_Rect const &b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool collisionDown(SDL_Rect const &a, Velocity const aVel, SDL_Rect const &b, Velocity const bVel);

bool collisionUp(SDL_Rect const &a, Velocity const aVel, SDL_Rect const &b, Velocity const bVel);

bool collisionLeft(SDL_Rect const &a, Velocity const aVel, SDL_Rect const &b, Velocity const bVel);

bool collisionRight(SDL_Rect const &a, Velocity const aVel, SDL_Rect const &b, Velocity const bVel);

bool collisionDownRight(SDL_Rect const &a, SDL_Rect const &b, Velocity const aVel);

bool collisionDownLeft(SDL_Rect const &a, SDL_Rect const &b, Velocity const aVel);

bool attackCollision(SDL_Rect const &a, SDL_Rect const &b);

}; // namespace Physics
#endif
