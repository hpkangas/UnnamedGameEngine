#include "gameobject.hpp"
#include "player.hpp"
#include "texturemanager.hpp"
#include "physics.cpp"
#include <iostream>
#include "camera.hpp"

Player::Player(const char *texturesheet, int x, int y, std::string name)
{
    this->name = name;
    objTexture = TextureManager::LoadTexture(texturesheet);

    xpos = x;
    ypos = y;
    xvel = 0;
    yvel = 0;

    flipType = SDL_FLIP_NONE;

    sourceRect.h = PLAYER_HEIGHT;
    sourceRect.w = PLAYER_WIDTH;
    sourceRect.x = 0;
    sourceRect.y = STILL_FRAME;

    destinationRect.x = x;
    destinationRect.y = y;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;

    collisionRect.h = PLAYER_HEIGHT;
    // baelog sprite offsets
    collisionRect.w = PLAYER_WIDTH - 56;
    collisionRect.x = xpos + 28;
    collisionRect.y = ypos;

    this->animationFrame = 1;
    WALKING_ANIMATION_FRAMES = 8;

    playerSpeed = 0.4f;
    playerSpeedMax = 8.0f;
}

Player::Player() {}
Player::~Player() {}

void Player::update()
{
    handleDownCollision();
    handleHorizontalCollision();
    handleGravityState();
    handleMovement();
    handleAnimationState();

    destinationRect.x = xpos;
    destinationRect.y = ypos;

    // baelog sprite offsets
    collisionRect.x = xpos + 28;
    collisionRect.y = ypos;
}

void Player::render(Camera camera)
{
    TextureManager::draw(objTexture, sourceRect, destinationRect, flipType, camera);
}

void Player::playFrame(int x, int y, int w, int h, int frame)
{
    sourceRect.h = h;
    sourceRect.w = w;
    sourceRect.x = x + w * frame;
    sourceRect.y = y;
}

void Player::moveRight()
{
    if (getAnimationState() != WALKING)
        setAnimationState(WALKING);

    setXvel(getXvel() + playerSpeed);

    if (getXvel() > playerSpeedMax)
        setXvel(playerSpeedMax);
}

void Player::moveLeft()
{
    if (getAnimationState() != WALKING)
        setAnimationState(WALKING);

    setXvel(getXvel() - playerSpeed);

    if (getXvel() < -playerSpeedMax)
        setXvel(-playerSpeedMax);
}

void Player::moveUp()
{
    setYvel(-playerSpeed);
}

// debugging only mostly
void Player::moveDown()
{
    setYvel(playerSpeed);
}

// debugging only mostly
void Player::stopX()
{
    if (getXvel() != 0)
    {
        setXvel(0);
        setAnimationState(STILL);
    }
}

// debugging only mostly
void Player::stopYU()
{
    if (this->yvel < 0)
    {
        this->yvel = 0;
        this->animationState = STILL;
    }
}

// debugging only mostly
void Player::stopYD()
{
    if (this->yvel > 0)
    {
        this->yvel = 0;
        this->animationState = STILL;
    }
}

void Player::gravity()
{
    if (getYvel() < MAX_GRAVITY)
    {
        setYvel(getYvel() + GRAVITYUNIT);
    }
}

void Player::handleMovement()
{
    float xvel = getXvel();
    float yvel = getYvel();
    int xpos = getXpos();
    int ypos = getYpos();

    if (!(xvel == 0))
    {
        setXpos(xpos + xvel);
    }

    if (!(yvel == 0))
    {
        setYpos(ypos + yvel);
    }

    if (xvel < 0)
    {
        flipType = SDL_FLIP_HORIZONTAL;
    }

    if (xvel > 0)
    {
        flipType = SDL_FLIP_NONE;
    }
}

void Player::handleDownCollision()
{
    if (getCollisionDown())
    {
        setGravityState(GROUNDED);
    }
    else
    {
        setGravityState(INAIR);
    }
}

void Player::handleHorizontalCollision()
{
    int xposDif = getXposDifferenceWithCollisionObject();

    if (this->getCollisionLeft())
    {
        if (xposDif != 0)
            setXpos(getXpos() - xposDif);
        setXvel(0);

        if (getGravityState() == GROUNDED)
            setAnimationState(PUSHING);
    }

    if (this->getCollisionRight())
    {
        if (xposDif != 0)
            setXpos(getXpos() - xposDif);
        setXvel(0);

        if (getGravityState() == GROUNDED)
            setAnimationState(PUSHING);
    }

    if (!this->getCollisionLeft() && !this->getCollisionRight())
    {
        if (getAnimationState() == PUSHING)
            setAnimationState(STILL);
    }
}
void Player::handleUpCollision(bool collision, int dif)
{
    // TODO
}

void Player::handleGravityState()
{
    switch (getGravityState())
    {
    case GROUNDED:
        if (getYposDifferenceWithCollisionObject() != 0)
            setYpos(getYpos() - getYposDifferenceWithCollisionObject());
        setYvel(0);
        if (getAnimationState() == anims_INAIR)
            setAnimationState(STILL);
        break;

    case INAIR:
        gravity();
        setAnimationState(anims_INAIR);
        break;

    default:
        break;
    }
}

void Player::handleAnimationState()
{
    switch (getAnimationState())
    {
    case STILL:
        this->animationFrame = 0;
        playFrame(0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, 0);
        break;

    case WALKING:
        playFrame(0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, this->animationFrame / WALKING_ANIMATION_FRAMES);
        this->animationFrame++;
        if (this->animationFrame / WALKING_ANIMATION_FRAMES > 7)
        {
            this->animationFrame = 0;
        }
        break;

    case PUSHING:
        // baelog animaatiossa destination rectille pitäisi olla ehkä joku offset
        playFrame(0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, this->animationFrame / 8);
        this->animationFrame++;
        if (this->animationFrame / 8 > 7)
        {
            this->animationFrame = 0;
        }
        break;

    case anims_INAIR:
        playFrame(0, INAIR_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, this->animationFrame / 8);
        this->animationFrame++;
        if (this->animationFrame / 8 > 1)
        {
            this->animationFrame = 0;
        }
        break;
    default:
        break;
    }
}
