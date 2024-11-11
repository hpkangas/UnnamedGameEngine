#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "constants.hpp"

class Player : public GameObject
{
public:
    Player(const char *texturesheet, int x, int y);
    Player();
    ~Player();
    playerState state;
    int animationFrame;
    int WALKING_ANIMATION_FRAMES;
    SDL_RendererFlip flipType;
    void playAnimation(SDL_Rect sourceRect);
    void playAnimation(SDL_Rect sourceRect, SDL_RendererFlip flip);
    void playFrame(int x, int y, int w, int h, int frame);

    void update();
    void render();
    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect;
    SDL_Rect collisionArea;

private:
};

#endif
