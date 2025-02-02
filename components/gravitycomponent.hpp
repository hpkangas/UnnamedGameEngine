#include "../constants.hpp"

struct GravityComponent {
  GravityState gravityState;

  GravityState getGravityState() const { return gravityState; }

  void setGravityState(const GravityState state) { gravityState = state; }
};