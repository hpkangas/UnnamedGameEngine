#include "texturemanager.hpp"
#include "SDL_error.h"
#include "SDL_render.h"
#include "system.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

Texture TextureManager::loadTexture(const std::string &fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName.c_str());

  Texture texture;

  if (tempSurface != nullptr) {
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));

    texture.texture = SDL_CreateTextureFromSurface(System::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (texture.texture == nullptr) {
      std::cout << "ERROR creating texture from " << fileName << ": " << SDL_GetError()
                << std::endl;
    }

    texture.name = fileName;
  } else {
    std::cout << "ERROR loading file " << fileName << ": " << IMG_GetError() << std::endl;
  }

  return texture;
}

void TextureManager::loadTextures() {
  textures.clear();
  textureIndexes.clear();

  std::vector<fs::path> texturePaths;

  for (const auto &dirEntry : fs::directory_iterator("assets")) {
    if (dirEntry.is_regular_file()) {
      texturePaths.emplace_back(dirEntry.path());
    }
  }

  std::ranges::sort(texturePaths);

  int i = 0;

  for (const fs::path &path : texturePaths) {
    const std::string fullPath = path.generic_string();
    const std::string fileName = path.filename().generic_string();

    Texture tex = TextureManager::loadTexture(fullPath);

    textures.emplace_back(tex);
    textureIndexes[fileName] = i;
    textureIndexes[fullPath] = i;

    i++;
  }
}

int TextureManager::getTextureIndex(const std::string &textureName) {
  auto it = textureIndexes.find(textureName);

  if (it != textureIndexes.end()) {
    return it->second;
  }

  std::cout << "TextureManager: texture not found: " << textureName << std::endl;

  auto fallback = textureIndexes.find("error.png");
  if (fallback != textureIndexes.end()) {
    return fallback->second;
  }

  return 0;
}

void TextureManager::clean() {
  for (Texture &texture : textures) {
    if (texture.texture != nullptr) {
      SDL_DestroyTexture(texture.texture);
      texture.texture = nullptr;
    }
  }

  textures.clear();
  textureIndexes.clear();
}
