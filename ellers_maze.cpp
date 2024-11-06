#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;
const int ROWS = 30;
const int COLS = 30;
const int CELL_SIZE = 20;

class Maze {
public:
    Maze(int rows, int cols) : rows(rows), cols(cols) {
        maze.resize(rows, std::vector<int>(cols, 1));
        set.resize(rows, std::vector<int>(cols, -1));
    }

    void generate() {
        for (int row = 0; row < rows; ++row) {
            if (row != 0) {
                // Remove horizontal walls between sets
                for (int col = 0; col < cols - 1; ++col) {
                    if (set[row][col] != set[row][col + 1]) {
                        maze[row][col] = 0;
                        set[row][col + 1] = set[row][col];
                    }
                }
            }

            // Randomly remove vertical walls
            for (int col = 0; col < cols; ++col) {
                if (rand() % 2 == 0 && col < cols - 1) {
                    maze[row][col] = 0;
                }
            }
        }
    }

    void draw(SDL_Renderer* renderer) {
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (maze[y][x] == 1) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }

private:
    int rows, cols;
    std::vector<std::vector<int>> maze;
    std::vector<std::vector<int>> set;
};

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Eller's Algorithm Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

int main() {
    srand(time(0));

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(&window, &renderer)) {
        return 1;
    }

    Maze maze(ROWS, COLS);
    maze.generate();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        maze.draw(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
