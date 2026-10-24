#pragma once
#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP
#include "sceneinfo.hpp"
#include "../utilities/utils.hpp"
using namespace Utils;

namespace EntityCreator {
  entt::entity createEntity(std::string templateName, Position position, Velocity velocity, SceneInfo &level);
  entt::entity createEntityOnTilePosition(std::string templateName, Position position, Velocity velocity, SceneInfo &level);
};

#endif