#include "rendersystem.hpp"
#include "../camera.hpp"
#include "../gamesystem.hpp"
#include <memory>

// void RenderSystem::update(Entity &player) {
//   player.GetComponent<RenderComponent>()->destinationRect.x =
//       player.GetComponent<TransformComponent>()->position.x;
//   player.GetComponent<RenderComponent>()->destinationRect.y =
//       player.GetComponent<TransformComponent>()->position.y;

//   if (player.GetComponent<TransformComponent>()->velocity.x < 0) {
//     player.GetComponent<RenderComponent>()->flip = SDL_FLIP_HORIZONTAL;
//   }

//   if (player.GetComponent<TransformComponent>()->velocity.x > 0) {
//     player.GetComponent<RenderComponent>()->flip = SDL_FLIP_NONE;
//   }
// }

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
    std::unordered_map<int, std::unique_ptr<RenderComponent>> &components) {

  SDL_RenderClear(GameSystem::renderer);
  for (auto &object : components) {
    draw(object.second->render());
  }

  SDL_RenderPresent(GameSystem::renderer);
}

void RenderSystem::render(RenderComponent &renderComponent) { draw(renderComponent.render()); }

void RenderSystem::draw(const RenderInfo &RenderInfo) {
  SDL_Rect newDestination = RenderInfo.destinationRect;
  newDestination.x -= Camera::m_offset.x;
  newDestination.y -= Camera::m_offset.y;
  SDL_RenderCopyEx(GameSystem::renderer, RenderInfo.objTexture, &RenderInfo.sourceRect,
                   &newDestination, 0, nullptr, RenderInfo.flip);
}
