#pragma once

#include <SDL2/SDL.h>
#include <string>

SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string path);
