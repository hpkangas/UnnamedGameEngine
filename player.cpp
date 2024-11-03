#include "gameobject.hpp"
#include "player.hpp"
#include "texturemanager.hpp"
#include <iostream>

Player::Player(const char *texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);

    xpos = x;
    ypos = y;
    flipType = SDL_FLIP_NONE;

    sourceRect.h = PLAYER_HEIGHT;
    sourceRect.w = PLAYER_WIDTH;
    sourceRect.x = 0;
    sourceRect.y = STILL_FRAME;

    destinationRect.x = x;
    destinationRect.y = y;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;

    collisionRect.w = PLAYER_HEIGHT;
    collisionRect.h = PLAYER_WIDTH;

    this->animationFrame = 1;
    WALKING_ANIMATION_FRAMES = 8;
}

Player::Player() {}
Player::~Player() {}

void Player::update()
{

    if (!(xvel == 0))
    {
        xpos += xvel;
    }

    if (!(yvel == 0))
    {
        ypos += yvel;
    }

    switch (state)
    {
    case STILL:
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
    default:
        break;
    }

    destinationRect.x = xpos;
    destinationRect.y = ypos;

    collisionRect.x = xpos;
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
