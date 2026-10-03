#pragma once
#ifndef SCENECREATOR_HPP
#define SCENECREATOR_HPP
#include "SDL_rwops.h"
#include "sceneinfo.hpp"

namespace SceneCreator {
SceneInfo createLevel(SDL_RWops *Handle);
}
#endif
