#include "gameobject.hpp"
#include "player.hpp"
#include "texturemanager.hpp"
#include <iostream>



Player::Player(const char *texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);


    

    xpos = x;
    ypos = y;
    int animationFrame = 0;

    const int WALKING_ANIMATION_FRAMES = 8;
    SDL_Rect walkClips[WALKING_ANIMATION_FRAMES];

    walkClips[0].x = 0;
    walkClips[0].y = 0;
    walkClips[0].w = 64;
    walkClips[0].h = 64;

    walkClips[1].x = 64;
    walkClips[1].y = 0;
    walkClips[1].w = 64;
    walkClips[1].h = 64;

    walkClips[2].x = 128;
    walkClips[2].y = 0;
    walkClips[2].w = 64;
    walkClips[2].h = 64;

    walkClips[3].x = 192;
    walkClips[3].y = 0;
    walkClips[3].w = 64;
    walkClips[3].h = 64;

    walkClips[4].x = 256;
    walkClips[4].y = 0;
    walkClips[4].w = 64;
    walkClips[4].h = 64;

    walkClips[5].x = 320;
    walkClips[5].y = 0;
    walkClips[5].w = 64;
    walkClips[5].h = 64;

    walkClips[6].x = 384;
    walkClips[6].y = 0;
    walkClips[6].w = 64;
    walkClips[6].h = 64;

    walkClips[7].x = 448;
    walkClips[7].y = 0;
    walkClips[7].w = 64;
    walkClips[7].h = 64;

}

Player::Player() {}
Player::~Player() {
}

void Player::update()
{
std::cout<<state;

    if (!(xvel == 0))
    {
        xpos += xvel;
    }

    if (!(yvel == 0))
    {
        ypos += yvel;
    }

    // height and width taken from image
    // x and y coordinates of image
    sourceRect.h = 32;
    sourceRect.w = 32;
    sourceRect.x = 0;
    sourceRect.y = 64;

    destinationRect.x = xpos;
    destinationRect.y = ypos;
    destinationRect.w = sourceRect.w;
    destinationRect.h = sourceRect.h;

    collisionRect.x = xpos;
    collisionRect.y = ypos;
    collisionRect.w = 320;
    collisionRect.h = 320;
}

void Player::render()
{
   switch (state)
   {
   case STILL:
   SDL_RenderCopy(Game::renderer, objTexture, &sourceRect, &destinationRect);
    break;
   case WALKING:
   //animate
   break;
   default:
    break;
   }
}

void Player::lowerHealth()
{
    health--;
}

