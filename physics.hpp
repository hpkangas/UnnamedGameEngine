#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "entities/entity.hpp"
#include <SDL2/SDL.h>

namespace Physics {

bool checkCollision(SDL_Rect &a, SDL_Rect b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool playerCollisionDown(Entity &player, Entity &obj);

bool playerCollisionUp(Entity &player, Entity &obj);

bool playerCollisionLeft(Entity &player, Entity &obj);

bool playerCollisionRight(Entity &player, Entity &obj);

}; // namespace Physics
#endif