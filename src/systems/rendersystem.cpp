#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../game.hpp"
#include <memory>

void RenderSystem::update(const TransformComponent &tc, RenderComponent &rc) {
  rc.destinationRect.x = tc.position.x;
  rc.destinationRect.y = tc.position.y;

  if (tc.velocity.x < 0) {
    rc.flip = SDL_FLIP_HORIZONTAL;
  }

  if (tc.velocity.x > 0) {
    rc.flip = SDL_FLIP_NONE;
  }
}

void RenderSystem::renderComponents(
    std::unordered_map<int, std::unique_ptr<RenderComponent>> &renderComponents) {

  SDL_RenderClear(Game::renderer);
  for (int i = renderComponents.size() - 1; i > -1; i--) {
    draw(renderComponents.at(i).get()->render());
  }
  // for (auto &object : renderComponents) {
  //   draw(object.second->render());
  // }

  SDL_RenderPresent(Game::renderer);
}

void RenderSystem::render(RenderComponent &renderComponent) { draw(renderComponent.render()); }

void RenderSystem::draw(const RenderInfo &RenderInfo) {
  SDL_Rect newDestination = RenderInfo.destinationRect;
  newDestination.x -= Camera::offset.x;
  newDestination.y -= Camera::offset.y;
  SDL_RenderCopyEx(Game::renderer, RenderInfo.objTexture, &RenderInfo.sourceRect, &newDestination,
                   0, nullptr, RenderInfo.flip);
}
