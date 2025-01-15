#include "texturemanager.hpp"
#include "game.hpp"

SDL_Texture *TextureManager::LoadTexture(const char *texture)
{
    SDL_Surface *tempSurface = IMG_Load(texture);

    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 220, 0, 212));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}
void TextureManager::draw(SDL_Texture *tex, SDL_Rect source, SDL_Rect destination, Camera camera)
{
    draw(tex, source, destination, SDL_FLIP_NONE, camera);
}

void TextureManager::draw(SDL_Texture *objTexture, SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_RendererFlip flip, Camera camera)
{
    SDL_Rect newDestination = destinationRect;
    newDestination.x -= camera.m_offset.x;
    newDestination.y -= camera.m_offset.y;
    SDL_RenderCopyEx(Game::renderer, objTexture, &sourceRect, &newDestination, 0, NULL, flip);
}