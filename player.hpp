#include "constants.hpp"
#include "gameobject.hpp"
#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player : public GameObject
{
public:
    Player(const char *texturesheet, int x, int y);
    Player();
    ~Player();
    float playerSpeed;
    float playerSpeedMax;
    int updatedX;

    bool canJump;

    int xpos;
    int ypos;
    float xvel;
    float yvel;

    GravityState gravityState;
    AnimationState animationState;
    int animationFrame;
    int WALKING_ANIMATION_FRAMES;
    SDL_RendererFlip flipType;
    void playFrame(int x, int y, int w, int h, int frame);

    void moveToX(int position);

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void stopX();
    void stopXR();
    void stopXL();
    void stopYU();
    void stopYD();

    void gravity();

    void update();
    void render();
    SDL_Texture *objTexture;
    SDL_Rect sourceRect, destinationRect, collisionRect;

private:
};

#endif
