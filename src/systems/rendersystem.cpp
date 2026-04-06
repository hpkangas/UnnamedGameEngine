#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../system.hpp"
#include "SDL_rect.h"
#include <memory>

void RenderSystem::update(SceneInfo &currentLevel) const {
  for (int id : currentLevel.objectRenderIds) {
    RenderComponent &rc = *currentLevel.renderComponents.at(id);
    TransformComponent const &tc = *currentLevel.transformsComponents.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;

    if (tc.velocity.x < 0) {
      rc.flip = SDL_FLIP_HORIZONTAL;
    }

    else if (tc.velocity.x > 0) {
      rc.flip = SDL_FLIP_NONE;
    }
  }

  for (int id : currentLevel.tileRenderIds) {
    RenderComponent &rc = *currentLevel.renderComponents.at(id);
    TransformComponent const &tc = *currentLevel.transformsComponents.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;
  }

  for (int id : currentLevel.UIRenderIds) {
    RenderComponent &rc = *currentLevel.renderComponents.at(id);
    TransformComponent const &tc = *currentLevel.transformsComponents.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;
  }
}

void RenderSystem::render(SceneInfo &currentLevel) const {
  std::unordered_map<int, std::unique_ptr<RenderComponent>> &renderComponents =
      currentLevel.renderComponents;

  SDL_RenderClear(System::renderer);

  for (int id : currentLevel.tileRenderIds) {
    renderNonStatic(renderComponents.at(id).get()->render());
  }
  for (int id : currentLevel.objectRenderIds) {
    renderNonStatic(renderComponents.at(id).get()->render());
  }

  for (int id : currentLevel.UIRenderIds) {
    renderStatic(renderComponents.at(id)->render());
  }

  SDL_RenderPresent(System::renderer);
}

void RenderSystem::renderStatic(const RenderInfo &renderInfo) const {

  SDL_RenderCopyEx(System::renderer, renderInfo.objTexture, &renderInfo.sourceRect,
                   &renderInfo.destinationRect, 0, nullptr, renderInfo.flip);
}

void RenderSystem::renderNonStatic(const RenderInfo &renderInfo) const {
  if (inView(renderInfo.destinationRect)) {
    SDL_Rect newDestination = renderInfo.destinationRect;
    newDestination.x -= Camera::offset.x;
    newDestination.y -= Camera::offset.y;

    SDL_RenderCopyEx(System::renderer, renderInfo.objTexture, &renderInfo.sourceRect,
                     &newDestination, 0, nullptr, renderInfo.flip);
  }
}

bool RenderSystem::inView(const SDL_Rect &destinationRect) const {
  return destinationRect.x + destinationRect.w >= Camera::viewPort.x &&
         destinationRect.y + destinationRect.h >= Camera::viewPort.y &&
         destinationRect.x <= Camera::viewPort.w && destinationRect.y <= Camera::viewPort.h;
}
