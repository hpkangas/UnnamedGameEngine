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

    playerSpeed = 4;
}

Player::Player() {}
Player::~Player() {}

void Player::update()
{
    //yvel = GRAVITY;
    
    // printf("updated %d\n", this->updatedX);
    if (!(this->xvel == 0))
    {
        this->xpos += this->xvel;
    }

    if (!(this->yvel == 0))
    {
        this->ypos += this->yvel;
    }

    switch (animationState)
    {
    case STILL:
        animationFrame = 0;
        // playerFrame args should be enums probably, or separate func playStill
        // need to think of a way to get sprite/frame information

        playFrame(0, STILL_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, 0);
        break;
    case WALKING:
        if (xvel < 0)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }

        playFrame(0, WALK_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationFrame / WALKING_ANIMATION_FRAMES);
        animationFrame++;
        if (animationFrame / WALKING_ANIMATION_FRAMES >= WALKING_ANIMATION_FRAMES)
        {
            animationFrame = 0;
        }

        break;
    case PUSHING:
        if (xvel < 0)
        {
            flipType = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flipType = SDL_FLIP_NONE;
        }
        // baelog animaatiossa destination rectille pitäisi olla ehkä joku offset
        playFrame(0, PUSH_FRAME, PLAYER_WIDTH, PLAYER_HEIGHT, animationFrame / 8);
        animationFrame++;
        if (animationFrame /8 >= 8)
        {
            animationFrame = 0;
        }
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
    playAnimation(sourceRect, flipType);
}

void Player::playAnimation(SDL_Rect sourceRect, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::renderer, objTexture, &sourceRect, &destinationRect, 0, NULL, flip);
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
    this->animationState = WALKING;
    this->xvel = playerSpeed;
    // updatedX = this->xvel;
}

void Player::moveLeft()
{
    this->animationState = WALKING;
    this->xvel = -playerSpeed;
    // updatedX = this->xvel;
}

void Player::moveUp()
{
    this->yvel = -playerSpeed;
}

void Player::moveDown()
{
    this->yvel = playerSpeed;
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