#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../system.hpp"
#include <memory>
#include <vector>

void RenderSystem::update(const TransformComponent &tc, RenderComponent &rc) const {
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
    std::unordered_map<int, std::unique_ptr<RenderComponent>> &renderComponents,
    std::vector<int> const &renderIds) const {

  SDL_RenderClear(System::renderer);
  for (int id : renderIds) {
    draw(renderComponents.at(id).get()->render());
  }
  // for (auto &object : renderComponents) {
  //   draw(object.second->render());
  // }

  SDL_RenderPresent(System::renderer);
}

void RenderSystem::render(RenderComponent &renderComponent) const {
  draw(renderComponent.render());
}

void RenderSystem::draw(const RenderInfo &RenderInfo) const {
  SDL_Rect newDestination = RenderInfo.destinationRect;
  newDestination.x -= Camera::offset.x;
  newDestination.y -= Camera::offset.y;
  SDL_RenderCopyEx(System::renderer, RenderInfo.objTexture, &RenderInfo.sourceRect, &newDestination,
                   0, nullptr, RenderInfo.flip);
}
