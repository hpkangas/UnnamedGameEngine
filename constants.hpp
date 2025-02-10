#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// TODO should be editable
constexpr int SCREENWIDTH = 1920;
constexpr int SCREENHEIGHT = 1080;

// TODO Think of better names
enum GAMEOBJECT_IDENTIFIERS { PC1 = 0, PC2 = 1, PC3 = 2 };

// TODO should be editable
constexpr int FRAMERATE = 60;

// TODO player character specific, not global constants
constexpr int PLAYER_HEIGHT = 128;
constexpr int PLAYER_WIDTH = 128;
constexpr int STILL_FRAME = 256;
constexpr int WALK_FRAME = 0;
constexpr int PUSH_FRAME = 640;
constexpr int INAIR_FRAME = 384;

#endif