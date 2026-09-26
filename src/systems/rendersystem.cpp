#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../system.hpp"
#include "../texturemanager.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"

void RenderSystem::update() const {
  auto &reg = currentLevel->registry;
  auto view = reg.view<RenderComponent, PositionComponent>();

  for (entt::entity id : view) {
    RenderComponent &rc = reg.get<RenderComponent>(id);
    PositionComponent const &tc = reg.get<PositionComponent>(id);

    rc.destinationRect.x = tc.position.x;
    rc.destinationRect.y = tc.position.y;

    if (tc.facingRight) {
      rc.flip = SDL_FLIP_NONE;
    } else {
      rc.flip = SDL_FLIP_HORIZONTAL;
    }
  }
}

void RenderSystem::render() const {
  auto &reg = currentLevel->registry;
  auto view = reg.view<RenderComponent, PositionComponent>();

  SDL_RenderClear(System::renderer);
  for (entt::entity id : view) {
    renderNonStatic(reg.get<RenderComponent>(id).render());
  }

  for (entt::entity id : currentLevel->UIRenderIds) {
    renderStatic(reg.get<RenderComponent>(id).render());
  }

  SDL_RenderPresent(System::renderer);
}

void RenderSystem::renderStatic(const RenderInfo &renderInfo) const {

  SDL_RenderCopyEx(System::renderer, TextureManager::textures.at(renderInfo.objTexture).texture,
                   &renderInfo.sourceRect, &renderInfo.destinationRect, 0, nullptr,
                   renderInfo.flip);
}

void RenderSystem::renderNonStatic(const RenderInfo &renderInfo) const {
  if (inView(renderInfo.destinationRect)) {
    SDL_Rect newDestination = renderInfo.destinationRect;
    newDestination.x -= Camera::offset.x;
    newDestination.y -= Camera::offset.y;

    SDL_RenderCopyEx(System::renderer, TextureManager::textures.at(renderInfo.objTexture).texture,
                     &renderInfo.sourceRect, &newDestination, 0, nullptr, renderInfo.flip);
  }
}

bool RenderSystem::inView(const SDL_Rect &destinationRect) const {
  return destinationRect.x + destinationRect.w >= Camera::viewPort.left &&
         destinationRect.y + destinationRect.h >= Camera::viewPort.top &&
         destinationRect.x <= Camera::viewPort.right &&
         destinationRect.y <= Camera::viewPort.bottom;
}
