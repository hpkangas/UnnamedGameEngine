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

void TextureManager::draw(RenderInfo renderInfo)
{
    SDL_Rect newDestination = renderInfo.destinationRect;
    newDestination.x -= renderInfo.camera.m_offset.x;
    newDestination.y -= renderInfo.camera.m_offset.y;
    SDL_RenderCopyEx(Game::renderer, renderInfo.objTexture, &renderInfo.sourceRect, &newDestination, 0, NULL, renderInfo.flip);
}