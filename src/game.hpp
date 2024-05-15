#pragma once

#include <SDL2/SDL.h>
#include <deque>
#include <string>

enum SnakeDirection {
    LEFT,
    RIGHT,
    DOWN,
    UP
};

struct SnakeSegment {
    int x, y;
};

struct Snake {
    SnakeDirection direction;
    SnakeSegment head;
    std::deque<SnakeSegment> bodySegments;
};

struct Food {
    int x, y;
};

class Game {

public:
    Game();

    void init(std::string windowTitle, int windowXPos, int windowYPos);
    void close();

    void startRunning();
    bool isRunning();

    void handleEvents();
    void update();

    void render();
    void renderSmpte();
    void renderDieScreen();

    bool didPlayerDie();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *smpte;

    bool running;
    bool playerDied;

    Snake snake;
    bool snakeDirectionChanged;
    bool snakeGrow;

    Food food;
};

