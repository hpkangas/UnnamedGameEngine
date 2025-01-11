#include "block.hpp"
#include "texturemanager.hpp"
#include "camera.hpp"

Block::Block(const char *texturesheet, int x, int y, int dx, int dy)
{
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = dx;
    ypos = dy;

    sourceRect.h = 64;
    sourceRect.w = 128;
    sourceRect.x = x;
    sourceRect.y = y;

    destinationRect.h = sourceRect.h;
    destinationRect.w = sourceRect.w;
    destinationRect.x = xpos;
    destinationRect.y = ypos;

    collisionRect.h = destinationRect.h;
    collisionRect.w = destinationRect.w;
    collisionRect.x = xpos;
    collisionRect.y = ypos + 4;

    // printCoordinates(sourceRect, destinationRect, collisionRect);
}

// Block::Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy)
// {
//     objTexture = TextureManager::LoadTexture(texturesheet);
//     sourceRect.h = h;
//     sourceRect.w = w;
//     sourceRect.x = x;
//     sourceRect.y = y;

//     destinationRect.h = sourceRect.h;
//     destinationRect.w = sourceRect.w;
//     destinationRect.x = dx;
//     destinationRect.y = dy;

//     collisionRect.h = destinationRect.h;
//     collisionRect.w = destinationRect.w;
//     collisionRect.x = destinationRect.x;
//     collisionRect.y = destinationRect.y;

//     // printCoordinates(sourceRect, destinationRect, collisionRect);
// }

void Block::update()
{
    destinationRect.x = xpos;
    destinationRect.y = ypos;

    // baelog sprite offsets
    collisionRect.x = xpos;
    collisionRect.y = ypos;
}

void Block::render(Camera camera)
{
    TextureManager::draw(this->objTexture, this->sourceRect, this->destinationRect, camera);
}
