#include "block.hpp"
#include "texturemanager.hpp"

Block::Block(const int x, const int y, const int sourceX) {
  m_position = {x, y};
  

  m_sourceRect.h = 64;
  m_sourceRect.w = 128;
  m_sourceRect.x = sourceX;
  m_sourceRect.y = 0;

  m_destinationRect.h = m_sourceRect.h;
  m_destinationRect.w = m_sourceRect.w;
  m_destinationRect.x = m_position.x;
  m_destinationRect.y = m_position.y;

  m_collisionRect.h = m_destinationRect.h;
  m_collisionRect.w = m_destinationRect.w;
  m_collisionRect.x = m_position.x;
  m_collisionRect.y = m_position.y + 4;
}

void Block::update() {
  m_destinationRect.x = m_collisionRect.x = m_position.x;
  m_destinationRect.y = m_collisionRect.y = m_position.y;
}

void Block::render(const Camera camera) {
  const RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, SDL_FLIP_NONE, camera};
  TextureManager::draw(info);
}
