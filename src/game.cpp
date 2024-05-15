#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "game.hpp"
#include "texture.hpp"

const int BORDER_SIZE = 10;

const int TILE_SIZE = 50;
const int HORIZONTAL_TILES = 20;
const int VERTICAL_TILES   = 20;

const int WINDOW_WIDTH  = 2 * BORDER_SIZE + TILE_SIZE * HORIZONTAL_TILES;
const int WINDOW_HEIGHT = 2 * BORDER_SIZE + TILE_SIZE * VERTICAL_TILES;

Game::Game() {}

void Game::init(std::string windowTitle, int windowXPos, int windowYPos) {
    window = nullptr;
    renderer = nullptr;
    smpte = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init error, %s\n", SDL_GetError());
        exit(-1);
    }

    window = SDL_CreateWindow(windowTitle.c_str(), windowXPos, windowYPos, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("SDL_CreateWindow error, %s\n", SDL_GetError());
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("SDL_CreateRenderer error, %s\n", SDL_GetError());
        exit(-1);
    }

    int imgInitFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgInitFlags) & imgInitFlags)) {
        printf("IMG_Init error, %s\n", SDL_GetError());
        exit(-1);
    }

    running = false;
    playerDied = false;

    smpte = loadTexture(renderer, "smpte.png");

    snake.direction = RIGHT;
    snakeDirectionChanged = true;
    snakeGrow = false;

    snake.head.x = 5;
    snake.head.y = VERTICAL_TILES / 2;

    food.x = rand() % HORIZONTAL_TILES;
    food.y = rand() % VERTICAL_TILES;

    for (int i = 1; i < 4; i++) {
        SnakeSegment segm = {snake.head.x - i, snake.head.y};
        snake.bodySegments.push_back(segm);
    }
}

void Game::close() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    IMG_Quit();
    SDL_Quit();

    running = false;
}

void Game::startRunning() {
    running = true;
}

bool Game::isRunning() {
    return running;
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_LEFT:
                    if (snake.direction != RIGHT && !snakeDirectionChanged) {
                        snake.direction = LEFT;
                        snakeDirectionChanged = true;
                    }
                    break;
                case SDLK_RIGHT:
                    if (snake.direction != LEFT && !snakeDirectionChanged) {
                        snake.direction = RIGHT;
                        snakeDirectionChanged = true;
                    }
                    break;
                case SDLK_UP:
                    if (snake.direction != DOWN && !snakeDirectionChanged) {
                        snake.direction = UP;
                        snakeDirectionChanged = true;
                    }
                    break;
                case SDLK_DOWN:
                    if (snake.direction != UP && !snakeDirectionChanged) {
                        snake.direction = DOWN;
                        snakeDirectionChanged = true;
                    }
                    break;
            }
        }
    }
}

void Game::update() {
    snake.bodySegments.pop_back();
    snake.bodySegments.push_front(snake.head);

    switch (snake.direction) {
        case LEFT:
            if (snake.head.x > 0) snake.head.x--;
            else playerDied = true;
            break;
        case RIGHT:
            if (snake.head.x < HORIZONTAL_TILES - 1) snake.head.x++;
            else playerDied = true;
            break;
        case UP:
            if (snake.head.y > 0) snake.head.y--;
            else playerDied = true;
            break;
        case DOWN:
            if (snake.head.y < VERTICAL_TILES - 1) snake.head.y++;
            else playerDied = true;
            break;
    }

    if (playerDied) {
        running = false;
        return;
    }

    snakeDirectionChanged = false;

    for (const auto &segment : snake.bodySegments) {
        if (snake.head.x == segment.x && snake.head.y == segment.y) {
            running = false;
            playerDied = true;
            return;
        }
    }

    if (snake.head.x == food.x && snake.head.y == food.y) {
        SnakeSegment grownSegment = snake.bodySegments.back();
        snake.bodySegments.push_back(grownSegment);

        bool foodPlacedOnSnakeBody = true;
        while (foodPlacedOnSnakeBody) {
            food.x = rand() % HORIZONTAL_TILES;
            food.y = rand() % VERTICAL_TILES;

            foodPlacedOnSnakeBody = false;

            for (const auto &segment : snake.bodySegments) {
                if (food.x == segment.x && food.y == segment.y) {
                    foodPlacedOnSnakeBody = true;
                    break;
                }
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 0xff);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 10, 10, 0xff);

    SDL_Rect borderRects[4] = {
        {0, 0, WINDOW_WIDTH, BORDER_SIZE},
        {0, 0, BORDER_SIZE, WINDOW_HEIGHT},
        {0, WINDOW_HEIGHT - BORDER_SIZE, WINDOW_WIDTH, BORDER_SIZE},
        {WINDOW_WIDTH - BORDER_SIZE, 0, BORDER_SIZE, WINDOW_HEIGHT}
    };

    SDL_RenderFillRects(renderer, borderRects, 4);

    SDL_Rect snakeRect = {snake.head.x * TILE_SIZE + BORDER_SIZE, snake.head.y * TILE_SIZE + BORDER_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0xff);
    SDL_RenderFillRect(renderer, &snakeRect);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0xff);
    for (const auto &segment : snake.bodySegments) {
        SDL_Rect segmentRect = {segment.x * TILE_SIZE + BORDER_SIZE, segment.y * TILE_SIZE + BORDER_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &segmentRect);
    }

    SDL_SetRenderDrawColor(renderer, 10, 100, 10, 0xff);
    SDL_Rect foodRect = {food.x * TILE_SIZE + BORDER_SIZE, food.y * TILE_SIZE + BORDER_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_RenderPresent(renderer);
    
    SDL_Delay(150 / log10(snake.bodySegments.size() + 10));
}

void Game::renderSmpte() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, smpte, nullptr, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(500);
}

void Game::renderDieScreen() {
    printf("%d\n", (int) snake.bodySegments.size());
    for (int i = 100; i >= 0; i -= 5) {
        SDL_SetRenderDrawColor(renderer, i, 10, 10, 0xff);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }
}

bool Game::didPlayerDie() {
    return playerDied;
}
