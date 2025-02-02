#include "block.hpp"
#include "../texturemanager.hpp"

Block::Block(const int x, const int y, const int sourceX) {
  transformComponent.position = {x, y};

  renderComponent.sourceRect.h = 64;
  renderComponent.sourceRect.w = 128;
  renderComponent.sourceRect.x = sourceX;
  renderComponent.sourceRect.y = 0;

  renderComponent.destinationRect.h = renderComponent.sourceRect.h;
  renderComponent.destinationRect.w = renderComponent.sourceRect.w;
  renderComponent.destinationRect.x = transformComponent.position.x;
  renderComponent.destinationRect.y = transformComponent.position.y;

  collisionComponent.collisionRect.h = renderComponent.destinationRect.h;
  collisionComponent.collisionRect.w = renderComponent.destinationRect.w;
  collisionComponent.collisionRect.x = transformComponent.position.x;
  collisionComponent.collisionRect.y = transformComponent.position.y + 4;

  renderComponent.objTexture = TextureManager::LoadTexture("assets/tiles.png");
}

void Block::update() {
  renderComponent.destinationRect.x = collisionComponent.collisionRect.x =
      transformComponent.position.x;
  renderComponent.destinationRect.y = collisionComponent.collisionRect.y =
      transformComponent.position.y;
}
