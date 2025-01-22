#include <SDL2/SDL_rwops.h>
#include <iostream>
#include <string>
namespace File {
inline void Read(const std::string &Path) {
  char *Content{static_cast<char *>(SDL_LoadFile(Path.c_str(), nullptr))};

  if (Content) {
    std::cout << "Content: " << Content;
  } else {
    std::cout << "Error loading file: " << SDL_GetError();
  }

  SDL_free(Content);
}

std::string ReadString(SDL_RWops *Handle) {
  std::string CurrentValue;
  char c;

  while (SDL_RWread(Handle, &c, 1, 1) == 1) {
    if (c == ',') {
      return CurrentValue;
    }
    CurrentValue += c;
  }
  return CurrentValue;
}

int ReadInt(SDL_RWops *Handle) { return std::stoi(ReadString(Handle)); }

float ReadFloat(SDL_RWops *Handle) { return std::stof(ReadString(Handle)); }

} // namespace File