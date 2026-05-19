#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../system.hpp"
#include "../texturemanager.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"

void RenderSystem::update() const {
  for (int id : currentLevel->objectRenderIds) {
    RenderComponent &rc = currentLevel->renderComponents.at(id);
    TransformComponent const &tc = currentLevel->transformComponents.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;

    if (tc.facingRight) {
      rc.flip = SDL_FLIP_NONE;
    } else {
      rc.flip = SDL_FLIP_HORIZONTAL;
    }
  }

  for (int id : currentLevel->tileRenderIds) {
    RenderComponent &rc = currentLevel->renderComponents.at(id);
    TransformComponent const &tc = currentLevel->transformComponents.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;
  }

  for (int id : currentLevel->UIRenderIds) {
    RenderComponent &rc = currentLevel->renderComponents.at(id);
    TransformComponent const &tc = currentLevel->transformComponents.at(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;
  }
}

void RenderSystem::render() const {
  auto &renderComponents = currentLevel->renderComponents;

  SDL_RenderClear(System::renderer);

  for (int id : currentLevel->tileRenderIds) {
    renderNonStatic(renderComponents.at(id).render());
  }
  for (int id : currentLevel->objectRenderIds) {
    renderNonStatic(renderComponents.at(id).render());
  }

  for (int id : currentLevel->UIRenderIds) {
    renderStatic(renderComponents.at(id).render());
  }

  SDL_RenderPresent(System::renderer);
}

void RenderSystem::renderStatic(const RenderInfo &renderInfo) const {

  SDL_RenderCopyEx(System::renderer, TextureManager::textures.at(renderInfo.objTexture),
                   &renderInfo.sourceRect, &renderInfo.destinationRect, 0, nullptr,
                   renderInfo.flip);
}

void RenderSystem::renderNonStatic(const RenderInfo &renderInfo) const {
  if (inView(renderInfo.destinationRect)) {
    SDL_Rect newDestination = renderInfo.destinationRect;
    newDestination.x -= Camera::offset.x;
    newDestination.y -= Camera::offset.y;

    SDL_RenderCopyEx(System::renderer, TextureManager::textures.at(renderInfo.objTexture),
                     &renderInfo.sourceRect, &newDestination, 0, nullptr, renderInfo.flip);
  }
}

bool RenderSystem::inView(const SDL_Rect &destinationRect) const {
  return destinationRect.x + destinationRect.w >= Camera::viewPort.left &&
         destinationRect.y + destinationRect.h >= Camera::viewPort.top &&
         destinationRect.x <= Camera::viewPort.right &&
         destinationRect.y <= Camera::viewPort.bottom;
}
