#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "constants.hpp"
#include "gameobject.hpp"
#include <string>

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
    Player(const char *texturesheet, int x, int y, std::string name);
    Player();
    ~Player();
    std::string name;

    SDL_Rect sourceRect, destinationRect, collisionRect;

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

    int getXpos()
    {
        return this->xpos;
    }

    int getYpos()
    {
        return this->ypos;
    }

    float getXvel()
    {
        return this->xvel;
    }

    float getYvel()
    {
        return this->yvel;
    }

    int getXposDifferenceWithCollisionObject()
    {
        return xposDif;
    }

    int getYposDifferenceWithCollisionObject()
    {
        return yposDif;
    }

    void setXpos(int xpos)
    {
        this->xpos = xpos;
    }

    void setYpos(int ypos)
    {
        this->ypos = ypos;
    }

    void setXvel(float xvel)
    {
        this->xvel = xvel;
    }

    void setYvel(float yvel)
    {
        this->yvel = yvel;
    }

    bool getCollisionDown()
    {
        return this->collisionDown;
    }

    bool getCollisionUp()
    {
        return this->collisionUp;
    }

    bool getCollisionRight()
    {
        return this->collisionRight;
    }

    bool getCollisionLeft()
    {
        return this->collisionLeft;
    }

    void setCollisionDown(bool collision)
    {
        this->collisionDown = collision;
    }

    void setCollisionUp(bool collision)
    {
        this->collisionUp = collision;
    }

    void setCollisionRight(bool collision)
    {
        this->collisionRight = collision;
    }

    void setCollisionLeft(bool collision)
    {
        this->collisionLeft = collision;
    }

    void setXposDifferenceWithCollisionObject(int xposDif)
    {
        this->xposDif = xposDif;
    }

    void setYposDifferenceWithCollisionObject(int yposDif)
    {
        this->yposDif = yposDif;
    }

    GravityState getGravityState()
    {
        return this->gravityState;
    }

    void setGravityState(GravityState gs)
    {
        this->gravityState = gs;
    }

    AnimationState getAnimationState()
    {
        return this->animationState;
    }

    void setAnimationState(AnimationState as)
    {
        this->animationState = as;
    }
};

class Baelog : public Player
{
public:
    using Player::Player;
    std::string name = "Baelog";
};

class Erik : public Player
{
public:
    using Player::Player;
    std::string name = "Erik";
};

class Olaf : public Player
{
public:
    using Player::Player;
    std::string name = "Olaf";
};

#endif
