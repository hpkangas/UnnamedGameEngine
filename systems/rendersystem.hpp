#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "../camera.hpp"
#include "../components/rendercomponent.hpp"
#include "../entities/entity.hpp"
#include <vector>

class RenderSystem {
public:
  RenderSystem() {}
  ~RenderSystem() {}
  RenderSystem(SDL_Renderer &renderer, Camera camera);
  void render(SDL_Renderer &renderer, Camera camera, std::vector<RenderComponent *> renderables);
  void render2(SDL_Renderer &renderer, Camera camera, RenderComponent &renderComponent);
  void update(Entity &player);
  SDL_Texture *LoadTexture(SDL_Renderer &renderer, const char *fileName);
  std::vector<SDL_Texture *> loadTextures(SDL_Renderer &renderer);
  void draw(SDL_Renderer &renderer, const RenderInfo &RenderInfo, const Camera &camera);
};

#endif