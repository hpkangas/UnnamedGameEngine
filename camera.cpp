#include "camera.hpp"
#include "constants.hpp"
#include "player.hpp"

Camera::Camera(){
    this->offset = {0, 0, 0, 0};
    this->position = {SCREENWIDTH * 1/3, SCREENHEIGHT * 1/3, SCREENWIDTH * 2/3, SCREENHEIGHT * 2/3};
}

SDL_Rect Camera::getPos()
{
    return this->position;
}

SDL_Rect Camera::getOffset()
{
    return this->offset;
}

void Camera::setPos(SDL_Rect rect)
{
    this->position = rect;
}

void Camera::setOffset(SDL_Rect rect)
{
    this->offset = rect;
}

void Camera::centerCameraToCharacter(Player *player)
{
    this->offset.x = player->getXpos() - SCREENWIDTH * 1 / 2;
    this->offset.y = player->getYpos() - SCREENHEIGHT * 1 / 2;

    Camera::updateCameraPosition();
}

void Camera::updateCameraPosition()
{
    this->position.x = SCREENWIDTH * 1 / 3 + this->offset.x;
    this->position.w = SCREENWIDTH * 2 / 3 + this->offset.x;
    this->position.y = SCREENHEIGHT * 1 / 3 + this->offset.y;
    this->position.h = SCREENHEIGHT * 2 / 3 + this->offset.y;
}

void Camera::updateCamera(Player *player)
{
    if (player->getXpos() < this->position.x && player->getXvel() < 0 ||
        player->getXpos() + PLAYER_WIDTH > this->position.w && player->getXvel() > 0)
    {
        this->offset.x += player->getXvel();
    }

    if (player->getYpos() < this->position.y && player->getYvel() < 0 ||
        player->getYpos() + PLAYER_HEIGHT > this->position.h && player->getYvel() > 0)
    {
        this->offset.y += player->getYvel();
    }
    Camera::updateCameraPosition();
}
