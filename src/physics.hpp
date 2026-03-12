#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "utils.hpp"
#include <SDL2/SDL.h>

using namespace Utils;
namespace Physics {

bool checkCollision(SDL_Rect &a, SDL_Rect b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool playerCollisionDown(SDL_Rect const &player, SDL_Rect const &obj,
                         Velocity const playerVelocity);

bool playerCollisionUp(SDL_Rect const &player, SDL_Rect const &obj,
                       Velocity const playerVelocity);

bool playerCollisionLeft(SDL_Rect const& player, SDL_Rect const &obj,
                         Velocity const playerVelocity);

bool playerCollisionRight(SDL_Rect const &player, SDL_Rect const &obj,
                          Velocity const    playerVelocity);

}; // namespace Physics
#endif
