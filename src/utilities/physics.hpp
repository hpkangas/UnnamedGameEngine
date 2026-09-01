#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "utils.hpp"
#include <SDL2/SDL.h>

using namespace Utils;
namespace Physics {

bool checkCollision(SDL_Rect const &a, SDL_Rect const &b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool collisionDown(SDL_Rect const &player, SDL_Rect const &obj, Velocity const playerVelocity);

bool collisionUp(SDL_Rect const &player, SDL_Rect const &obj, Velocity const playerVelocity);

bool collisionLeft(SDL_Rect const &player, SDL_Rect const &obj, Velocity const playerVelocity);

bool collisionRight(SDL_Rect const &player, SDL_Rect const &obj, Velocity const playerVelocity);

bool attackCollision(SDL_Rect const &a, SDL_Rect const &b);

}; // namespace Physics
#endif
