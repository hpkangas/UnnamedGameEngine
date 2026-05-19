#pragma once
#ifndef TILEGRID_HPP
#define TILEGRID_HPP

#include <cstddef>
#include <vector>

// Flat 2D array indexed by (tileX, tileY). Stores the entity id of the
// static collider occupying each cell, or `EMPTY` if the cell has none.
//
// Used by CollisionDetectionSystem to replace the O(N*M) "every dynamic
// entity vs every static tile" loop with a localized neighbour query.
// Tile coordinates that fall outside the grid silently resolve to EMPTY
// so callers don't need explicit bounds-checking around the query loop.
struct TileGrid {
  static constexpr int EMPTY = -1;

  int width  = 0;
  int height = 0;
  std::vector<int> ids;

  void resize(int w, int h) {
    width  = w;
    height = h;
    ids.assign(static_cast<std::size_t>(w) * static_cast<std::size_t>(h), EMPTY);
  }

  int at(int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) return EMPTY;
    return ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width)
               + static_cast<std::size_t>(tx)];
  }

  void set(int tx, int ty, int id) {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) return;
    ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width)
        + static_cast<std::size_t>(tx)] = id;
  }
};

#endif

