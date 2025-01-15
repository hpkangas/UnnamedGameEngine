#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "constants.hpp"
#include "gameobject.hpp"
#include <string>

class Player : public GameObject
{
private:
    std::string m_name;

    int m_xpos;
    int m_ypos;
    float m_xvel;
    float m_yvel;

    float m_playerSpeed;
    float m_playerSpeedMax;

    int m_yposDif;
    int m_xposDif;

    bool m_collisionUp = false;
    bool m_collisionDown = false;
    bool m_collisionLeft = false;
    bool m_collisionRight = false;

    GravityState m_gravityState;
    AnimationState m_animationState;
    int m_animationFrame;
    int m_WALKING_ANIMATION_FRAMES;

    SDL_RendererFlip m_flipType;
    SDL_Texture *m_objTexture;

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


    SDL_Rect m_sourceRect, m_destinationRect, m_collisionRect;

    void playFrame(int x, int y, int w, int h, int frame);

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void stopX();
    void stopYU();
    void stopYD();

    void update();
    void render(Camera camera);

    const int getXpos()
    {
        return m_xpos;
    }

    const int getYpos()
    {
        return m_ypos;
    }

    const float getXvel()
    {
        return m_xvel;
    }

    const float getYvel()
    {
        return m_yvel;
    }

    const int getXposDifferenceWithCollisionObject()
    {
        return m_xposDif;
    }

    const int getYposDifferenceWithCollisionObject()
    {
        return m_yposDif;
    }

    void setXpos(int xpos)
    {
        m_xpos = xpos;
    }

    void setYpos(int ypos)
    {
        m_ypos = ypos;
    }

    void setXvel(float xvel)
    {
        m_xvel = xvel;
    }

    void setYvel(float yvel)
    {
        m_yvel = yvel;
    }

    const bool getCollisionDown()
    {
        return m_collisionDown;
    }

    const bool getCollisionUp()
    {
        return m_collisionUp;
    }

    const bool getCollisionRight()
    {
        return m_collisionRight;
    }

    const bool getCollisionLeft()
    {
        return m_collisionLeft;
    }

    void setCollisionDown(bool collision)
    {
        m_collisionDown = collision;
    }

    void setCollisionUp(bool collision)
    {
        m_collisionUp = collision;
    }

    void setCollisionRight(bool collision)
    {
        m_collisionRight = collision;
    }

    void setCollisionLeft(bool collision)
    {
        m_collisionLeft = collision;
    }

    void setXposDifferenceWithCollisionObject(int xposDif)
    {
        m_xposDif = xposDif;
    }

    void setYposDifferenceWithCollisionObject(int yposDif)
    {
        m_yposDif = yposDif;
    }

    const GravityState getGravityState()
    {
        return m_gravityState;
    }

    void setGravityState(GravityState gs)
    {
        m_gravityState = gs;
    }

    const AnimationState getAnimationState()
    {
        return m_animationState;
    }

    void setAnimationState(AnimationState as)
    {
        m_animationState = as;
    }

    const std::string getName(){
        return m_name;
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
