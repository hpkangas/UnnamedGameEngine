#include "block.hpp"
#include "texturemanager.hpp"

Block::Block(const char *texturesheet, int x, int y, int dx, int dy){
    objTexture = TextureManager::LoadTexture(texturesheet);
    sourceRect.h = 64;
    sourceRect.w = 128;
    sourceRect.x = x;
    sourceRect.y = y;

    destinationRect.h = sourceRect.h;
    destinationRect.w = sourceRect.w;
    destinationRect.x = dx; 
    destinationRect.y = dy;

    collisionRect.h = destinationRect.h;
    collisionRect.w = destinationRect.w;
    collisionRect.x = destinationRect.x;
    collisionRect.y = destinationRect.y + 4;

   // printCoordinates(sourceRect, destinationRect, collisionRect);
}

Block::Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy){
    objTexture = TextureManager::LoadTexture(texturesheet);
    sourceRect.h = h;
    sourceRect.w = w;
    sourceRect.x = x;
    sourceRect.y = y;

    destinationRect.h = sourceRect.h;
    destinationRect.w = sourceRect.w;
    destinationRect.x = dx;
    destinationRect.y = dy;

    collisionRect.h = destinationRect.h;
    collisionRect.w = destinationRect.w;
    collisionRect.x = destinationRect.x;
    collisionRect.y = destinationRect.y;

   // printCoordinates(sourceRect, destinationRect, collisionRect);
}

void Block::update()
{
}

void Block::render()
{
    TextureManager::draw(this->objTexture, this->sourceRect, this->destinationRect);
}

