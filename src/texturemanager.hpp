#pragma once
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include <vector>
#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

struct Texture {
  SDL_Texture *texture = nullptr;
  std::string name;
  int id = -1;
};

struct TextureManager {
  inline static std::vector<Texture> textures;
  inline static std::unordered_map<std::string, int> textureIndexes;

  static void loadTextures();
  static void clean();

  static Texture loadTexture(const std::string &fileName);
  static int getTextureIndex(const std::string &textureName);
};
// namespace TextureManager

#endif
