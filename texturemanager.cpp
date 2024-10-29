#include "texturemanager.hpp"

SDL_Texture *TextureManager::LoadTexture(const char *texture)
{
    SDL_Surface *tempSurface = IMG_Load(texture);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);

    SDL_FreeSurface(tempSurface);

    return tex;
}
void TextureManager::draw(SDL_Texture *tex, SDL_Rect source, SDL_Rect destination)
{
    SDL_RenderCopy(Game::renderer, tex, &source, &destination);
}
