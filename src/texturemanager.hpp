#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#pragma once
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include <vector>

struct Texture {
  SDL_Texture *texture = nullptr;
  std::string name;
};

namespace TextureManager {
inline std::vector<Texture> textures;
inline std::unordered_map<std::string, int> textureIndexes;

void loadTextures();
void clean();

Texture loadTexture(const std::string &fileName);
int getTextureIndex(const std::string &textureName);
}; // namespace TextureManager
// namespace TextureManager

#endif
