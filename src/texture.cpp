#include "texture.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());

    if (texture == nullptr) {
        printf("IMG_LoadTexture %s error, %s\n", path.c_str(), SDL_GetError());   
        exit(-1); 
    }

    return texture;
}
