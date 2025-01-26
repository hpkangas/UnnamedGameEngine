#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "block.hpp"
#include "player.hpp"
#include <SDL2/SDL.h>
// namespace?
namespace Physics {

bool checkCollision(SDL_Rect &a, SDL_Rect b);

bool checkLeft(SDL_Rect a, SDL_Rect b);

bool playerCollisionDown(const Player &player, const Block &obj);

bool playerCollisionUp(const Player &player, const Block &obj);

bool playerCollisionLeft(const Player &player, const Block &obj);

bool playerCollisionRight(const Player &player, const Block &obj);

}; // namespace Physics
#endif