#include "texturemanager.hpp"
#include "camera.hpp"

SDL_Texture *TextureManager::LoadTexture(const char *texture)
{
    SDL_Surface *tempSurface = IMG_Load(texture);

    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}
void TextureManager::draw(SDL_Texture *tex, SDL_Rect source, SDL_Rect destination)
{
    SDL_Rect newDestination = destination;
    newDestination.x -= Camera::offset.x;
    newDestination.y -= Camera::offset.y;
    SDL_RenderCopy(Game::renderer, tex, &source, &newDestination);
}

void TextureManager::drawAnimation(SDL_Texture *objTexture, SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_RendererFlip flip)
{
    SDL_Rect newDestination = destinationRect;
    newDestination.x -= Camera::offset.x;
    newDestination.y -= Camera::offset.y;
    SDL_RenderCopyEx(Game::renderer, objTexture, &sourceRect, &newDestination, 0, NULL, flip);
}