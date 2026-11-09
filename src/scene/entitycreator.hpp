#pragma once
#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP
#include "../utilities/utils.hpp"
#include "sceneinfo.hpp"
using namespace Utils;

namespace EntityCreator {
entt::entity createEntity(std::string templateName, Position position, SceneInfo &level);
entt::entity createEntity(std::string templateName, Position position, Velocity velocity,
                          SceneInfo &level);
entt::entity createEntityOnTilePosition(std::string templateName, Position position,
                                        SceneInfo &level);
}; // namespace EntityCreator

#endif
