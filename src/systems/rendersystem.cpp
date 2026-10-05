#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../components/rendercomponent.hpp"
#include "../components/positioncomponent.hpp"
#include "../system.hpp"
#include "../texturemanager.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include <algorithm>
#include <vector>

void RenderSystem::update() const {
  auto &reg = currentLevel->registry;
  auto view = reg.view<RenderComponent, PositionComponent>();

  for (entt::entity id : view) {
    RenderComponent &rc = view.get<RenderComponent>(id);
    PositionComponent const &tc = view.get<PositionComponent>(id);

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

  //Sort entities by render layer to ensure correct rendering order
  std::vector<entt::entity> entities(view.begin(), view.end());
  std::stable_sort(entities.begin(), entities.end(), [&view](entt::entity lhs, entt::entity rhs) {
    return view.get<RenderComponent>(lhs).renderLayer > view.get<RenderComponent>(rhs).renderLayer;
  });

  for (entt::entity id : entities) {
    renderNonStatic(view.get<RenderComponent>(id).render());
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
