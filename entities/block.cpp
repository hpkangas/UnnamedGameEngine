#include "block.hpp"
#include "../texturemanager.hpp"

Block::Block(const int x, const int y, const int sourceX) {
  tc.m_position = {x, y};

  rc.m_sourceRect.h = 64;
  rc.m_sourceRect.w = 128;
  rc.m_sourceRect.x = sourceX;
  rc.m_sourceRect.y = 0;

  rc.m_destinationRect.h = rc.m_sourceRect.h;
  rc.m_destinationRect.w = rc.m_sourceRect.w;
  rc.m_destinationRect.x = tc.m_position.x;
  rc.m_destinationRect.y = tc.m_position.y;

  m_collisionRect.h = rc.m_destinationRect.h;
  m_collisionRect.w = rc.m_destinationRect.w;
  m_collisionRect.x = tc.m_position.x;
  m_collisionRect.y = tc.m_position.y + 4;

  rc.m_objTexture = TextureManager::LoadTexture("assets/tiles.png");
}

void Block::update() {
  rc.m_destinationRect.x = m_collisionRect.x = tc.m_position.x;
  rc.m_destinationRect.y = m_collisionRect.y = tc.m_position.y;
}

// void Block::render(const Camera camera) {
//   const RenderInfo info = {m_objTexture, m_sourceRect, m_destinationRect, SDL_FLIP_NONE, camera};
//   TextureManager::draw(info);
// }
