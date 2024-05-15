#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.hpp"

int main(int argc, char* argv[]) {
    (void) argc; (void) argv;

    srand(time(NULL));

    Game *game = new Game();

    game->init("Snake in SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    game->startRunning();
    game->renderSmpte();

    while (game->isRunning()) {
        game->handleEvents();
        game->update();
        game->render();
    }

    if (game->didPlayerDie()) game->renderDieScreen();

    game->close();

    // if (borderCollision) {
    //     printf("YOU TOUCHED THE BORDER\n");
    // }

    return 0;
}
