#pragma once
#include "components/component.hpp"
#include <memory>
#include <unordered_map>
#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "components/rendercomponent.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/gameobject.hpp"
#include "entities/player.hpp"
#include <vector>

using ComponentVector = std::vector<std::unique_ptr<IComponent>>;

struct LevelInfo {
  std::vector<GameObject *> gameobjects;
  std::vector<Entity *> playercharacters;
  std::vector<Block *> blocks;
  std::vector<RenderComponent *> renderables;
  std::vector<Entity *> entities;
  std::unordered_map<int, ComponentVector> entitieswip;
};

class Level {
public:
  Level() {};
  ~Level() {};
  Player *createPlayerCharacter(std::string const object, int const x, int const y);
  Block *createBlock(std::string const object, int const x, int const y);
  LevelInfo createLevel(SDL_RWops *Handle);

  Block *createBlock1(int x, int y);
  Block *createBlock2(int x, int y);
  Block *createBlock3(int x, int y);

  ComponentVector createBlockEntity2(int x, int y);
  std::vector<IComponent *> createPCEntity2(int x, int y);
  Entity *createBlockEntity(int x, int y);
  Entity *createPCEntity(int x, int y);

  void add(IComponent *component);
  int getID();

}; // namespace Level

#endif