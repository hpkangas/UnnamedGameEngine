#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../game.hpp"
#include <memory>
#include <vector>

void RenderSystem::update(const TransformComponent &tc, RenderComponent &rc) {
  rc.destinationRect.x = tc.getXpos();
  rc.destinationRect.y = tc.getYpos();

  if (tc.getXvel() < 0) {
    rc.flip = SDL_FLIP_HORIZONTAL;
  }

  if (tc.getXvel() > 0) {
    rc.flip = SDL_FLIP_NONE;
  }
}

void RenderSystem::renderComponents(
    std::unordered_map<int, std::unique_ptr<RenderComponent>> &renderComponents,
    std::vector<int> renderIds) {

  SDL_RenderClear(Game::renderer);
  for (int id : renderIds) {
    draw(renderComponents.at(id).get()->render());
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
