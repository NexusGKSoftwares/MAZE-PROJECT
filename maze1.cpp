#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

// Constants for the window and maze dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 20;
const int MAZE_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int MAZE_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Directions for moving in the maze
const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

// Cell structure to represent each cell in the maze
struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true};
};

std::vector<std::vector<Cell>> maze(MAZE_WIDTH, std::vector<Cell>(MAZE_HEIGHT));

// SDL Window and Renderer
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Function to initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Maze Generator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// Function to draw the maze
void drawMaze() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int x = 0; x < MAZE_WIDTH; ++x) {
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            Cell& cell = maze[x][y];
            int x1 = x * CELL_SIZE;
            int y1 = y * CELL_SIZE;

            if (cell.walls[0]) SDL_RenderDrawLine(renderer, x1, y1, x1 + CELL_SIZE, y1);             // Top
            if (cell.walls[1]) SDL_RenderDrawLine(renderer, x1 + CELL_SIZE, y1, x1 + CELL_SIZE, y1 + CELL_SIZE); // Right
            if (cell.walls[2]) SDL_RenderDrawLine(renderer, x1 + CELL_SIZE, y1 + CELL_SIZE, x1, y1 + CELL_SIZE); // Bottom
            if (cell.walls[3]) SDL_RenderDrawLine(renderer, x1, y1 + CELL_SIZE, x1, y1);             // Left
        }
    }

    SDL_RenderPresent(renderer);
}

// Function to generate the maze using a different algorithm
void generateMaze() {
    std::srand(std::time(0));
    std::stack<std::pair<int, int>> stack;
    int x = 0, y = 0;
    maze[x][y].visited = true;
    stack.push({x, y});

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();
        std::vector<int> directions = {0, 1, 2, 3};
        std::random_shuffle(directions.begin(), directions.end());

        bool moved = false;
        for (int i : directions) {
            int nx = cx + DX[i];
            int ny = cy + DY[i];
            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !maze[nx][ny].visited) {
                maze[cx][cy].walls[i] = false;
                maze[nx][ny].walls[i ^ 1] = false;
                maze[nx][ny].visited = true;
                stack.push({nx, ny});
                moved = true;
                break;
            }
        }
        if (!moved) stack.pop();
    }
}

// Function to clean up SDL resources
void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    if (!initSDL()) return -1;

    generateMaze();
    drawMaze();

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
    }

    cleanUp();
    return 0;
}
