#pragma once
#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP
#include "sceneinfo.hpp"
#include "../utilities/utils.hpp"
using namespace Utils;

namespace EntityCreator {
  void createBlock(std::string const &object, Position position, SceneInfo &level);
  void createBlock1(Position position, SceneInfo &level);
  void createBlock2(Position position, SceneInfo &level);
  void createBlock3(Position position, SceneInfo &level);
  void createEmptyTile(Position position, SceneInfo &level);
  void createBlockEntity(Position position, int sourceX, SceneInfo &level);
  void createSemiSolidBlockEntity(Position position, int sourceX, SceneInfo &level);
  void createLadder(Position position, SceneInfo &level);
  void createLadderStart(Position position, SceneInfo &level);
  entt::entity createPC(Position position, SceneInfo &level);
  entt::entity createEnemy(Position position, SceneInfo &level);
  void createUI(Position position, SceneInfo &level);
  entt::entity createProjectile(std::string templateName, Position position, Velocity velocity, SceneInfo &level);
};

#endif