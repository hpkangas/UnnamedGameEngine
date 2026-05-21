#pragma once
#ifndef TILEGRID_HPP
#define TILEGRID_HPP

#include "../include/entt/entt.hpp"
#include <cstddef>
#include <vector>

// Flat 2D array indexed by (tileX, tileY). Stores the entity of the
// static collider occupying each cell, or `entt::null` if the cell has none.
struct TileGrid {
  int width = 0;
  int height = 0;
  std::vector<entt::entity> ids;

  void resize(int w, int h) {
    width = w;
    height = h;
    ids.assign(static_cast<std::size_t>(w) * static_cast<std::size_t>(h),
               static_cast<entt::entity>(entt::null));
  }

  entt::entity at(int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return static_cast<entt::entity>(entt::null);
    }
    return ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width) +
               static_cast<std::size_t>(tx)];
  }

  void set(int tx, int ty, entt::entity id) {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return;
    }
    ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width) +
        static_cast<std::size_t>(tx)] = id;
  }
};

#endif
