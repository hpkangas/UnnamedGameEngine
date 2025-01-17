#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

//TODO should be editable 
const int SCREENWIDTH = 1920;
const int SCREENHEIGHT = 1080;

// TODO Think of better names
enum AnimationState
{
    STILL = 0,
    WALKING = 1,
    PUSHING = 2,
    anims_INAIR = 3
};
enum GravityState
{
    GROUNDED = 0,
    INAIR = 1,
    FALLING = 2
};

// TODO should be editable
const int FRAMERATE = 60;

const float GRAVITYUNIT = 0.50f;
const float MAX_GRAVITY = 11.0;

//TODO player character specific, not global constants
const int PLAYER_HEIGHT = 128;
const int PLAYER_WIDTH = 128;
const int STILL_FRAME = 256;
const int WALK_FRAME = 0;
const int PUSH_FRAME = 640;
const int INAIR_FRAME = 384;

#endif