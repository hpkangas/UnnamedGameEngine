#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "components/collisioncomponent.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>

using namespace Utils;
namespace Physics {

bool checkCollision(SDL_Rect &a, SDL_Rect b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool playerCollisionDown(CollisionComponent &player, CollisionComponent &obj,
                         Velocity playerVelocity);

bool playerCollisionUp(CollisionComponent &player, CollisionComponent &obj,
                       Velocity playerVelocity);

bool playerCollisionLeft(CollisionComponent &player, CollisionComponent &obj,
                         Velocity playerVelocity);

bool playerCollisionRight(CollisionComponent &player, CollisionComponent &obj,
                          Velocity playerVelocity);

}; // namespace Physics
#endif
