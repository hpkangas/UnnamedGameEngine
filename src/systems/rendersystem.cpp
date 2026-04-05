#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../system.hpp"
#include <memory>

void RenderSystem::update(SceneInfo &currentLevel) const {
  for (int id : currentLevel.ObjectRenderIds) {
    RenderComponent &rc = *currentLevel.rcs.at(id);
    TransformComponent const &tc = *currentLevel.tcs.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;

    if (tc.velocity.x < 0) {
      rc.flip = SDL_FLIP_HORIZONTAL;
    }

    else if (tc.velocity.x > 0) {
      rc.flip = SDL_FLIP_NONE;
    }
  }

  for (int id : currentLevel.TileRenderIds) {
    RenderComponent &rc = *currentLevel.rcs.at(id);
    TransformComponent const &tc = *currentLevel.tcs.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;
  }

  for (int id : currentLevel.UIRenderIds) {
    RenderComponent &rc = *currentLevel.rcs.at(id);
    TransformComponent const &tc = *currentLevel.tcs.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;
  }
}

void RenderSystem::renderComponents(SceneInfo &currentLevel) const {
  std::unordered_map<int, std::unique_ptr<RenderComponent>> &renderComponents = currentLevel.rcs;

  SDL_RenderClear(System::renderer);

  for (int id : currentLevel.ObjectRenderIds) {
    renderWithCamera(renderComponents.at(id).get()->render());
  }

  for (int id : currentLevel.TileRenderIds) {
    renderWithCamera(renderComponents.at(id).get()->render());
  }

  for (int id : currentLevel.UIRenderIds) {
    render(renderComponents.at(id)->render());
  }

  SDL_RenderPresent(System::renderer);
}

void RenderSystem::render(const RenderInfo &ri) const {
  SDL_RenderCopyEx(System::renderer, ri.objTexture, &ri.sourceRect, &ri.destinationRect, 0, nullptr,
                   ri.flip);
}

void RenderSystem::renderWithCamera(const RenderInfo &RenderInfo) const {
  SDL_Rect newDestination = RenderInfo.destinationRect;
  newDestination.x -= Camera::offset.x;
  newDestination.y -= Camera::offset.y;
  SDL_RenderCopyEx(System::renderer, RenderInfo.objTexture, &RenderInfo.sourceRect, &newDestination,
                   0, nullptr, RenderInfo.flip);
}
