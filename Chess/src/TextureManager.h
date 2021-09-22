#pragma once
#include "SDL.h"
#include "SDL_image.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path);
};
