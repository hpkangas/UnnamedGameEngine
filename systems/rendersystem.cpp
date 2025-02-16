#include "rendersystem.hpp"
#include <iostream>
#include <ostream>
void RenderSystem::update(Entity &player) {
  player.GetComponent<RenderComponent>()->destinationRect.x =
      player.GetComponent<TransformComponent>()->position.x;
  player.GetComponent<RenderComponent>()->destinationRect.y =
      player.GetComponent<TransformComponent>()->position.y;

  if (player.GetComponent<TransformComponent>()->velocity.x < 0) {
    player.GetComponent<RenderComponent>()->flip = SDL_FLIP_HORIZONTAL;
  }

  if (player.GetComponent<TransformComponent>()->velocity.x > 0) {
    player.GetComponent<RenderComponent>()->flip = SDL_FLIP_NONE;
  }
}
void RenderSystem::render(SDL_Renderer &renderer, Camera camera,
                          std::vector<RenderComponent *> renderables) {

  SDL_RenderClear(&renderer);
  for (auto &object : renderables) {
    draw(renderer, object->render(), camera);
  }

  SDL_RenderPresent(&renderer);
}

void RenderSystem::render2(SDL_Renderer &renderer, Camera camera,
                           RenderComponent &renderComponent) {

  draw(renderer, renderComponent.render(), camera);
}

SDL_Texture *RenderSystem::LoadTexture(SDL_Renderer &renderer, const char *fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName);
  if (tempSurface != nullptr) {
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(&renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
  }
  std::cout << "ERROR LOADING TEXTURE " << fileName << std::endl;
  return nullptr;
}

std::vector<SDL_Texture *> RenderSystem::loadTextures(SDL_Renderer &renderer) {
  std::vector<SDL_Texture *> textures;
  textures.emplace_back(LoadTexture(renderer, "../assets/tiles.png"));
  textures.emplace_back(LoadTexture(renderer, "../assets/baleog.png"));
  return textures;
};

void RenderSystem::draw(SDL_Renderer &renderer, const RenderInfo &RenderInfo,
                        const Camera &camera) {
  SDL_Rect newDestination = RenderInfo.destinationRect;
  newDestination.x -= camera.m_offset.x;
  newDestination.y -= camera.m_offset.y;
  SDL_RenderCopyEx(&renderer, RenderInfo.objTexture, &RenderInfo.sourceRect, &newDestination, 0,
                   nullptr, RenderInfo.flip);
}