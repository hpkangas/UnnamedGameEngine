#ifndef UTILS_HPP
#define UTILS_HPP
namespace Utils {

struct Position {
  int x;
  int y;

  bool operator<(const Position &other) const {
    return (x < other.x) || (x == other.x && y < other.y);
  }
  Position operator+(const Position &other) const { return Position(x + other.x, y + other.y); }
  Position operator-(const Position &other) const { return Position(x - other.x, y - other.y); }
};

struct Velocity {
  float x;
  float y;

  Velocity operator+(const Velocity &other) const { return Velocity(x + other.x, y + other.y); }
  Velocity operator+(const int &other) const { return Velocity(x + other, y + other); }
  Velocity operator-(const Velocity &other) const { return Velocity(x - other.x, y - other.y); }

  void add(int xvel, int yvel) {
    x += xvel;
    y += yvel;
  }
};

} // namespace Utils
#endif
