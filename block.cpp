#include "block.hpp"
#include "texturemanager.hpp"

Block::Block(const char *texturesheet, int x, int y, int dx, int dy)
{
    m_objTexture = TextureManager::LoadTexture(texturesheet);
    m_xpos = dx;
    m_ypos = dy;

    m_sourceRect.h = 64;
    m_sourceRect.w = 128;
    m_sourceRect.x = x;
    m_sourceRect.y = y;

    m_destinationRect.h = m_sourceRect.h;
    m_destinationRect.w = m_sourceRect.w;
    m_destinationRect.x = m_xpos;
    m_destinationRect.y = m_ypos;

    m_collisionRect.h = m_destinationRect.h;
    m_collisionRect.w = m_destinationRect.w;
    m_collisionRect.x = m_xpos;
    m_collisionRect.y = m_ypos + 4;

    // printCoordinates(m_sourceRect, m_destinationRect, m_collisionRect);
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
    m_destinationRect.x = m_xpos;
    m_destinationRect.y = m_ypos;

    m_collisionRect.x = m_xpos;
    m_collisionRect.y = m_ypos;
}

void Block::render(Camera camera)
{
    TextureManager::draw(this->m_objTexture, this->m_sourceRect, this->m_destinationRect, camera);
}
