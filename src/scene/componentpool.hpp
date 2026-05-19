#pragma once
#ifndef COMPONENTPOOL_HPP
#define COMPONENTPOOL_HPP

#include <cstddef>
#include <utility>
#include <vector>

// Sparse-set component storage. Drop-in replacement for ComponentMap<T>:
// every method ComponentMap exposes is present with matching semantics,
// so existing call sites compile unchanged. The two wins over the
// hash-map backend are:
//
//   * Lookups are `dense[sparse[id]]` — two array indices, no hash.
//   * Iteration walks `dense` contiguously, which removes the need for
//     the parallel `std::vector<int>` id lists in SceneInfo.
//
// Iteration semantic differs from std::unordered_map: range-based for
// yields `T&` (not `pair<const int, T>&`). No current call site
// iterates the pool directly via range-for, so this is a non-issue.
template <typename T>
class ComponentPool {
  static constexpr int NONE = -1;

  std::vector<T>   dense;          // packed components — iterate this
  std::vector<int> denseToEntity;  // dense[i] belongs to denseToEntity[i]
  std::vector<int> sparse;         // sparse[eid] = index into dense, or NONE

public:
  ComponentPool() {
    dense.reserve(256);
    denseToEntity.reserve(256);
  }
  explicit ComponentPool(std::size_t hint) {
    dense.reserve(hint);
    denseToEntity.reserve(hint);
  }

  void reserve(std::size_t n) {
    dense.reserve(n);
    denseToEntity.reserve(n);
  }

  // --- ComponentMap-compatible API --------------------------------------

  template <class... Args>
  T &emplace(int id, Args &&...args) {
    if (id >= 0 && id < static_cast<int>(sparse.size()) && sparse[id] != NONE) {
      return dense[sparse[id]];
    }
    if (id >= static_cast<int>(sparse.size())) {
      sparse.resize(id + 1, NONE);
    }
    sparse[id] = static_cast<int>(dense.size());
    denseToEntity.push_back(id);
    dense.emplace_back(std::forward<Args>(args)...);
    return dense.back();
  }

  void remove(int id) {
    if (id < 0 || id >= static_cast<int>(sparse.size()) || sparse[id] == NONE) {
      return;
    }
    const int idx  = sparse[id];
    const int last = static_cast<int>(dense.size()) - 1;
    if (idx != last) {
      dense[idx]                 = std::move(dense[last]);
      denseToEntity[idx]         = denseToEntity[last];
      sparse[denseToEntity[idx]] = idx;
    }
    dense.pop_back();
    denseToEntity.pop_back();
    sparse[id] = NONE;
  }

  bool contains(int id) const {
    return id >= 0 && id < static_cast<int>(sparse.size()) && sparse[id] != NONE;
  }

  T *get(int id) {
    return contains(id) ? &dense[sparse[id]] : nullptr;
  }
  T const *get(int id) const {
    return contains(id) ? &dense[sparse[id]] : nullptr;
  }

  // Pre-condition: contains(id). Two array accesses, no hashing.
  T       &at(int id)       { return dense[sparse[id]]; }
  T const &at(int id) const { return dense[sparse[id]]; }

  std::size_t size()  const { return dense.size(); }
  bool        empty() const { return dense.empty(); }

  // Range-based for over components only. Use entityAt() in tandem with
  // operator[] when you also need the entity id.
  auto begin()       { return dense.begin(); }
  auto end()         { return dense.end(); }
  auto begin() const { return dense.begin(); }
  auto end()   const { return dense.end(); }

  // --- Dense access (used when iterating the pool directly) -------------

  T       &operator[](std::size_t i)       { return dense[i]; }
  T const &operator[](std::size_t i) const { return dense[i]; }

  int entityAt(std::size_t i) const { return denseToEntity[i]; }
};

#endif

