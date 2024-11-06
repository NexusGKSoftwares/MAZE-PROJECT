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
        visited.resize(rows, std::vector<bool>(cols, false));
    }

    void generate(int startX, int startY) {
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        std::pair<int, int> current = {startX, startY};
        visited[startY][startX] = true;
        maze[startY][startX] = 0;

        while (true) {
            std::random_shuffle(directions.begin(), directions.end());
            bool moved = false;

            for (auto dir : directions) {
                int nx = current.first + dir.first;
                int ny = current.second + dir.second;

                if (nx >= 0 && ny >= 0 && nx < cols && ny < rows && !visited[ny][nx]) {
                    maze[ny][nx] = 0;
                    visited[ny][nx] = true;
                    current = {nx, ny};
                    moved = true;
                    break;
                }
            }

            if (!moved) {
                break;
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
    std::vector<std::vector<bool>> visited;
};

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Aldous-Broder Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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
    maze.generate(0, 0);

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
