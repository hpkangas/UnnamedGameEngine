#pragma once
#ifndef COMPONENTMAP_HPP
#define COMPONENTMAP_HPP

#include <cstddef>
#include <unordered_map>
#include <utility>

// A thin wrapper around std::unordered_map<int, T> tailored to the ECS
// access patterns in this project. Two things are gained over the raw
// `std::unordered_map<int, std::unique_ptr<T>>` it replaces:
//
//   * `get(id)` returns a nullable pointer with a single hash lookup,
//     collapsing the common `contains(id) + at(id)` pair into one probe.
//   * Components are stored inline (no `std::unique_ptr` indirection),
//     so a lookup followed by a field access touches one cache line
//     instead of two and avoids a per-component heap allocation.
//
// The interface is intentionally small so the backing storage can later
// be swapped (e.g. for a sparse set) without touching call sites.
template <typename T>
class ComponentMap {
  std::unordered_map<int, T> data;

public:
  ComponentMap() { data.reserve(256); }
  explicit ComponentMap(std::size_t initialBuckets) { data.reserve(initialBuckets); }

  void reserve(std::size_t n) { data.reserve(n); }

  // Construct a component in place, forwarding args to T's constructor.
  // Returns a reference to the (possibly newly-inserted) component.
  template <class... Args>
  T &emplace(int id, Args &&...args) {
    auto [it, inserted] = data.try_emplace(id, std::forward<Args>(args)...);
    return it->second;
  }

  void remove(int id) { data.erase(id); }

  // Nullable accessor — one hash lookup, no exception path.
  T *get(int id) {
    auto it = data.find(id);
    return it == data.end() ? nullptr : &it->second;
  }
  T const *get(int id) const {
    auto it = data.find(id);
    return it == data.end() ? nullptr : &it->second;
  }

  // Asserting accessor — use when the caller already knows the id is present.
  T       &at(int id)       { return data.at(id); }
  T const &at(int id) const { return data.at(id); }

  bool        contains(int id) const { return data.contains(id); }
  std::size_t size() const           { return data.size(); }

  auto begin()       { return data.begin(); }
  auto end()         { return data.end(); }
  auto begin() const { return data.begin(); }
  auto end()   const { return data.end(); }
};

#endif

