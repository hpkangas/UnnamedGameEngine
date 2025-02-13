#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include <SDL2/SDL.h>

namespace Physics {

bool checkCollision(SDL_Rect &a, SDL_Rect b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool playerCollisionDown(const Entity &player, const Block &obj);

bool playerCollisionUp(const Entity &player, const Block &obj);

bool playerCollisionLeft(const Entity &player, const Block &obj);

bool playerCollisionRight(const Entity &player, const Block &obj);

}; // namespace Physics
#endif