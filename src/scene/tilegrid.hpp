#pragma once
#ifndef TILEGRID_HPP
#define TILEGRID_HPP

#include "../include/entt/entt.hpp"
#include "../utilities/utils.hpp"

// Flat 2D array indexed by (tileX, tileY). Stores the entity of the
// static collider occupying each cell, or `entt::null` if the cell has none.
using Utils::Position;
struct TileGrid {
  int width = 0;
  int height = 0;
  // std::vector<entt::entity> ids;
  // Physics::Position ennemmin? kuin pair
  std::map<Position, std::vector<entt::entity>> entityCellPositions;

  void resize(int w, int h) {
    width = w;
    height = h;
  }

  std::vector<entt::entity> at(int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return std::vector<entt::entity>{entt::null};
    }
    Position pos{tx, ty};
    if (entityCellPositions.find(pos) == entityCellPositions.end()) {
      return std::vector<entt::entity>{entt::null};
    }
    return entityCellPositions.at(pos);
  }

  std::vector<Position> getCells(entt::entity id) const {
    std::vector<Position> cells;
    for (const auto &it : entityCellPositions) {
      for (const auto entity : it.second) {
        if (entity == id) {
          cells.emplace_back(it.first);
        }
      }
    }
    return cells;
  }

  void set(int tx, int ty, entt::entity id) {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
      return;
    }
    if (entityCellPositions.find(Position(tx, ty)) == entityCellPositions.end()) {
      entityCellPositions.insert({Position(tx, ty), std::vector<entt::entity>{id}});
    } else {
      entityCellPositions[Position(tx, ty)].emplace_back(id);
    }
  }

  void addTileIfEmpty(int tx, int ty, entt::entity entity){
  if (at(tx, ty).at(0) == entt::null) {
    set(tx, ty, entity);
  }}

};

#endif
