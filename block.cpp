#include "block.hpp"
#include "texturemanager.hpp"

//each type of terrain should probably be its own class with only position as its parameters
Block::Block(const char *texturesheet, int x, int y, int dx, int dy)
{
    m_objTexture = TextureManager::LoadTexture(texturesheet);
    m_position = {dx, dy};

    m_sourceRect.h = 64;
    m_sourceRect.w = 128;
    m_sourceRect.x = x;
    m_sourceRect.y = y;

    m_destinationRect.h = m_sourceRect.h;
    m_destinationRect.w = m_sourceRect.w;
    m_destinationRect.x = m_position.x;
    m_destinationRect.y = m_position.y;

    m_collisionRect.h = m_destinationRect.h;
    m_collisionRect.w = m_destinationRect.w;
    m_collisionRect.x = m_position.x;
    m_collisionRect.y = m_position.y + 4;
}

// Block::Block(const char *texturesheet, int x, int y, int h, int w, int dx, int dy)
// {
//     m_objTexture = TextureManager::LoadTexture(texturesheet);
//     m_sourceRect.h = h;
//     m_sourceRect.w = w;
//     m_sourceRect.x = x;
//     m_sourceRect.y = y;

//     m_destinationRect.h = m_sourceRect.h;
//     m_destinationRect.w = m_sourceRect.w;
//     m_destinationRect.x = dx;
//     m_destinationRect.y = dy;

//     m_collisionRect.h = m_destinationRect.h;
//     m_collisionRect.w = m_destinationRect.w;
//     m_collisionRect.x = m_destinationRect.x;
//     m_collisionRect.y = m_destinationRect.y;

//     // printCoordinates(m_sourceRect, m_destinationRect, m_collisionRect);
// }

void Block::update()
{
    m_destinationRect.x = m_collisionRect.x = m_position.x;
    m_destinationRect.y = m_collisionRect.y = m_position.y;
}

void Block::render(Camera camera)
{
    RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, SDL_FLIP_NONE, camera};
    TextureManager::draw(info);
}
