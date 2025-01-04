#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "constants.hpp"
#include "gameobject.hpp"

class Player : public GameObject
{
private:
    int xpos;
    int ypos;
    float xvel;
    float yvel;

    float playerSpeed;
    float playerSpeedMax;

    int yposDif;
    int xposDif;

    bool collisionUp = false;
    bool collisionDown = false;
    bool collisionLeft = false;
    bool collisionRight = false;

    GravityState gravityState;
    AnimationState animationState;
    int animationFrame;
    int WALKING_ANIMATION_FRAMES;
    
    SDL_RendererFlip flipType;
    SDL_Texture *objTexture;

    void gravity();
    void handleMovement();
    void handleDownCollision();
    void handleHorizontalCollision();
    void handleUpCollision(bool collision, int dif);
    void handleGravityState();
    void handleAnimationState();

public:
    Player(const char *texturesheet, int x, int y);
    Player();
    ~Player();
    
    SDL_Rect sourceRect, destinationRect, collisionRect;

    int getXpos();
    int getYpos();
    float getXvel();
    float getYvel();
    int getXposDifferenceWithCollisionObject();
    int getYposDifferenceWithCollisionObject();

    bool getCollisionDown();
    bool getCollisionUp();
    bool getCollisionRight();
    bool getCollisionLeft();

    void setXpos(int xpos);
    void setYpos(int ypos);
    void setXvel(float xvel);
    void setYvel(float yvel);

    void setCollisionDown(bool collision);
    void setCollisionUp(bool collision);
    void setCollisionRight(bool collision);
    void setCollisionLeft(bool collision);

    void setXposDifferenceWithCollisionObject(int xposDif);
    void setYposDifferenceWithCollisionObject(int yposDif); 

    GravityState getGravityState();
    AnimationState getAnimationState();

    void setGravityState(GravityState gs);
    void setAnimationState(AnimationState as);


    void playFrame(int x, int y, int w, int h, int frame);

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void stopX();
    void stopYU();
    void stopYD();

    void update();
    void render();
};

#endif
