#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "components/component.hpp"
#include "entities/entity.hpp"
#include <memory>
#include <vector>

using ComponentVector = std::vector<std::unique_ptr<IComponent>>;
using EntityVector = std::vector<std::unique_ptr<Entity>>;

struct LevelInfo {
  EntityVector entities;
};

class Level {
public:
  Level() {};
  ~Level() {};
  LevelInfo createLevel(SDL_RWops *Handle);

  std::unique_ptr<Entity> createBlock(std::string const object, int const x, int const y);
  std::unique_ptr<Entity> createBlock1(int x, int y);
  std::unique_ptr<Entity> createBlock2(int x, int y);
  std::unique_ptr<Entity> createBlock3(int x, int y);
  std::unique_ptr<Entity> createBlockEntity(int x, int y, int sourceX);

  std::unique_ptr<Entity> createPCEntity(int x, int y);

}; // namespace Level

#endif