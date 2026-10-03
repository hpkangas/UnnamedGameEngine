
#pragma once
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants {

// TODO should be editable
constexpr int SCREENWIDTH = 1920;
constexpr int SCREENHEIGHT = 1080;
constexpr int FRAMERATE = 60;
constexpr double FRAME_MS = 1000.0 / FRAMERATE;
constexpr double MAX_LAG_MS = 250.0;

// TODO player character specific, not global constants
constexpr int PLAYER_INDEX = 1;
constexpr int PLAYER_HEIGHT = 128;
constexpr int PLAYER_WIDTH = 128;
constexpr int TILE_SIZE = 128;
} // namespace Constants

#endif
