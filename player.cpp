#include "gameobject.hpp"
#include "player.hpp"
#include "texturemanager.hpp"
#include "physics.cpp"
#include <iostream>

Player::Player(const char *texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);

    xpos = x;
    ypos = y;
    xvel = 0;
    yvel = 0;

    updatedX;

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
    //  this->gravityState = GROUNDED;
}

Player::Player() {}
Player::~Player() {}

void Player::update()
{

    gravity();

    printf("gs %d\n", this->gravityState);
    printf("yvel %f\n", this->yvel);
    if (!(this->xvel == 0))
    {
        this->xpos += (int)this->xvel;
    }

    if (!(this->yvel == 0))
    {
        this->ypos += (int)this->yvel;
    }

    if (xvel < 0)
    {
        flipType = SDL_FLIP_HORIZONTAL;
    }
     if (xvel > 0)
    {
        flipType = SDL_FLIP_NONE;
    }

    switch (animationState)
    {
    case STILL:
        animationFrame = 0;
        playFrame(0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, 0);
        break;

    case WALKING:
        playFrame(0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationFrame / WALKING_ANIMATION_FRAMES);
        animationFrame++;
        if (animationFrame / WALKING_ANIMATION_FRAMES > 7)
        {
            animationFrame = 0;
        }
        break;

    case PUSHING:
        // baelog animaatiossa destination rectille pitäisi olla ehkä joku offset
        playFrame(0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationFrame / 8);
        animationFrame++;
        if (animationFrame / 8 > 7)
        {
            animationFrame = 0;
        }
        break;

    case anims_INAIR:
        playFrame(0, INAIR_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationFrame / 8);
        animationFrame++;
        if (animationFrame / 8 > 1)
        {
            animationFrame = 0;
        }
        break;
    default:
        break;
    }

    destinationRect.x = xpos;
    destinationRect.y = ypos;

    // baelog sprite offsets
    collisionRect.x = xpos + 28;
    collisionRect.y = ypos;
}

void Player::render()
{
    TextureManager::drawAnimation(objTexture, sourceRect, destinationRect, flipType);
}

void Player::playFrame(int x, int y, int w, int h, int frame)
{
    sourceRect.h = h;
    sourceRect.w = w;
    sourceRect.x = x + w * frame;
    sourceRect.y = y;
}

void Player::moveToX(int position)
{
    updatedX = position;
}

void Player::moveRight()
{
    if (!animationState == WALKING)
        this->animationState = WALKING;

    this->xvel += playerSpeed;

    if (xvel > playerSpeedMax)
        xvel = playerSpeedMax;
}

void Player::moveLeft()
{
    if (!animationState == WALKING)
        this->animationState = WALKING;

    this->xvel -= playerSpeed;

    if (xvel < -playerSpeedMax)
        xvel = -playerSpeedMax;
}

void Player::moveUp()
{
    this->yvel = -playerSpeed;
}

void Player::moveDown()
{
    this->yvel = playerSpeed;
}

void Player::stopX()
{
    if (this->xvel != 0)
    {
        this->xvel = 0;
        this->animationState = STILL;
    }
}

void Player::stopXR()
{
    if (this->xvel > 0)
    {
        this->xvel = 0;
        this->animationState = STILL;
    }
}
void Player::stopXL()
{
    if (this->xvel < 0)
    {
        this->xvel = 0;
        this->animationState = STILL;
    }
}

void Player::stopYU()
{
    if (this->yvel < 0)
    {
        this->yvel = 0;
        this->animationState = STILL;
    }
}

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
    if (this->gravityState != GROUNDED)
    {

        if (this->yvel <= MAX_GRAVITY)
        {
            this->yvel += 0.50f;
        }
    }
}