#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "game.hpp"
#include "constants.hpp"

class Player : public GameObject
{
public:
    Player(const char *texturesheet, int x, int y);
    Player();
    ~Player();

    playerState state;

    void lowerHealth();
    int health;
    bool isVulnerable = true;
    bool dead = false;
    int timeWhenShot;
    int timeWhenHit;
    int timeSinceHit;

    void update();
    void render();
    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect;
    SDL_Rect collisionArea;

private:
};

#endif
