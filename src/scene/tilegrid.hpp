#pragma once
#ifndef TILEGRID_HPP
#define TILEGRID_HPP

#include "../include/entt/entt.hpp"
#include <algorithm>
#include <cstddef>
#include <vector>

// Flat 2D array indexed by (tileX, tileY). Each cell stores the entities
// whose collision rectangles overlap it.
struct TileGrid {
  int width = 0;
  int height = 0;
  std::vector<std::vector<entt::entity>> ids;

  void resize(int w, int h) {
    width = w;
    height = h;
    ids.clear();
    ids.resize(static_cast<std::size_t>(w) * static_cast<std::size_t>(h));
  }

  std::vector<entt::entity> const &at(int tx, int ty) const {
    static const std::vector<entt::entity> empty;
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return empty;
    }
    return ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width) +
               static_cast<std::size_t>(tx)];
  }

  void add(int tx, int ty, entt::entity id) {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return;
    }
    auto &cell = ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width) +
                      static_cast<std::size_t>(tx)];
    if (id != entt::null && std::find(cell.begin(), cell.end(), id) == cell.end()) {
      cell.push_back(id);
    }
  }

  void remove(int tx, int ty, entt::entity id) {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return;
    }
    auto &cell = ids[static_cast<std::size_t>(ty) * static_cast<std::size_t>(width) +
                     static_cast<std::size_t>(tx)];
    cell.erase(std::remove(cell.begin(), cell.end(), id), cell.end());
  }

  void addRectangle(int x, int y, int w, int h, int cellSize, entt::entity id) {
    forRectangleCells(x, y, w, h, cellSize, [this, id](int tx, int ty) {
      add(tx, ty, id);
    });
  }

  void removeRectangle(int x, int y, int w, int h, int cellSize, entt::entity id) {
    forRectangleCells(x, y, w, h, cellSize, [this, id](int tx, int ty) {
      remove(tx, ty, id);
    });
  }

private:
  template <typename Callback>
  static void forRectangleCells(int x, int y, int w, int h, int cellSize,
                                Callback callback) {
    if (cellSize <= 0) {
      return;
    }

    const int right = x + std::max(w, 1) - 1;
    const int bottom = y + std::max(h, 1) - 1;
    const int minTx = floorDivide(x, cellSize);
    const int maxTx = floorDivide(right, cellSize);
    const int minTy = floorDivide(y, cellSize);
    const int maxTy = floorDivide(bottom, cellSize);

    for (int ty = minTy; ty <= maxTy; ++ty) {
      for (int tx = minTx; tx <= maxTx; ++tx) {
        callback(tx, ty);
      }
    }
  }

  static int floorDivide(int value, int divisor) {
    const int quotient = value / divisor;
    return value % divisor < 0 ? quotient - 1 : quotient;
  }
};

#endif
